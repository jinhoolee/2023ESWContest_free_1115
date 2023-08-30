#include "custom_filesystem.h"
#include "custom_exception.h"
#include <string.h>

// +1 is just for safety.
uint8_t filesystem[FILESYSTEM_SIZE + 1] = { 0 };
bool filesystemLoaded = false;

#define FSBASE	filesystem
#define FSEND	(FSBASE+FILESYSTEM_SIZE)

#define PTR(x) 	((uint8_t*)(x))
#define FILE(x)	((FileInfo_t*)(x))
#define SIZE(x) (FILEINFO_SIZE + (FILE(x))->size)
#define DATA(x)	(PTR(x)+FILEINFO_SIZE)

#define NAME_ISVALID(x)		(Custom_FileSystem_IsValidName(x))
#define RANGE_ISVALID(x)	((FSBASE<=PTR(x))&&((PTR(x)+SIZE(x))<FSEND))

typedef union {
	FileInfo_t fileInfo;
	uint8_t bytes[FILEINFO_SIZE];
} FileInfo_u;

void Custom_FileSystem_Load() {
	Custom_Flash_Read(filesystem, FILESYSTEM_SIZE);
	filesystemLoaded = true;
}

void Custom_FileSystem_Reset() {
	for (int i = 0; i < FILESYSTEM_SIZE; i++) {
		filesystem[i] = 0;
	}
}

bool Custom_FileSystem_Flush() {
	ASSERT_MSG(filesystemLoaded, "File system is not loaded yet");
	ASSERT(Custom_Flash_Erase());
	ASSERT(Custom_Flash_Write(filesystem, FILESYSTEM_SIZE));
	return true;
}

static uint8_t Custom_FileSystem_Get_CheckSum(FileInfo_t *file) {
	FileInfo_u *fileUnion = (FileInfo_u*) file;

	uint8_t checksum = 0;

	for (int i = 0; i < FILEINFO_SIZE; i++) {
		checksum ^= fileUnion->bytes[i];
	}

	return checksum;
}

bool Custom_FileSystem_IsValidName(char *filename) {
	// 파일 이름이 너무 짧은 경우(0글자)
	ASSERT_MSG(filename[0] != 0, "Filename is too short.");

	// 파일 이름이 적절한 경우
	for (int i = 0; i < FILENAME_LENGTH; i++) {
		if (filename[i] == 0) return true;
	}

	// 파일 이름이 너무 긴 경우
	ASSERT_MSG(false, "Filename is too long.");
}

bool Custom_FileSystem_Validate(FileInfo_t *file) {
	ASSERT_MSG(filesystemLoaded, "File system is not loaded yet.");
	ASSERT(NAME_ISVALID(file->filename));
	ASSERT_MSG(RANGE_ISVALID(file), "File out out file system.");
	ASSERT_MSG(file->size > 0, "File size is 0.");

	uint8_t checksum = Custom_FileSystem_Get_CheckSum(file);
	ASSERT_MSG(checksum == 0, "Checksum is not 0.");
	return true;
}

FileInfo_t* Custom_FileSystem_Find(char *filename) {
	if (!filesystemLoaded) {
		// 아예 파일시스템 로드가 안 된 경우
		SET_ERR("File system is not loaded yet.");
		return FILEINFO_NULL;
	}

	if (!NAME_ISVALID(filename)) return FILEINFO_NULL;

	uint8_t *file = filesystem;
	while (true) {
		if (!Custom_FileSystem_Validate(FILE(file))) break;
		if (strcmp((FILE(file))->filename, filename) == 0) return FILE(file);
		else file += SIZE(file);
	}

	SET_ERR("Such file does not exist.");
	return FILEINFO_NULL;
}

bool Custom_FileSystem_Delete(FileInfo_t *file) {
	ASSERT(Custom_FileSystem_Validate(file));

	// 파일데이터를 shift한다.
	uint8_t *ptr1 = PTR(file);
	uint8_t *ptr2 = ptr1 + SIZE(file);

	for (; ptr2 < FSEND;) {
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
	}

	// 남는 공간을 0으로 채운다.
	for (; ptr1 < FSEND; ptr1++) {
		*ptr1 = 0;
	}

	return true;
}

bool Custom_FileSystem_Read(FileInfo_t *file, uint8_t *dest, uint32_t length) {
	ASSERT(Custom_FileSystem_Validate(file));
	ASSERT_MSG(file->size >= length, "Given length is larger than file size.");

	uint8_t *ptr = DATA(file);
	for (uint32_t i = 0; i < length; i++) {
		dest[i] = ptr[i];
	}

	return true;
}

bool Custom_FileSystem_Write(char *filename, uint8_t *data, uint32_t length) {
	ASSERT_MSG(filesystemLoaded, "File system is not loaded yet.");
	ASSERT(NAME_ISVALID(filename));
	ASSERT_MSG(length > 0, "Cannot write zero length file.");

	// 모든 파일을 순차적으로 읽으면서 남은 저장공간 계산
	uint32_t freeSpace = FILESYSTEM_SIZE;
	uint8_t *file = FSBASE;
	while (Custom_FileSystem_Validate(FILE(file))) {
		uint8_t size = SIZE(FILE(file));
		freeSpace -= size;
		file += size;
	}

	// 파일 크기의 합과 마지막 파일의 주소가 같은지 검사
	ASSERT_MSG(
			(FILESYSTEM_SIZE + FSBASE) == (file + freeSpace),
			"File system free space unmatched."
			);

	// 같은 이름의 파일이 있을 경우 덮어쓸 것이므로 용량 증가
	FileInfo_t *exists = Custom_FileSystem_Find(filename);

	if (exists != FILEINFO_NULL) {
		uint32_t size = SIZE(exists);
		freeSpace += size;
		file -= size;
	}

	// 파일이 너무 커서 저장 불가능할 경우 예외처리
	ASSERT_MSG(freeSpace >= FILEINFO_SIZE + length, "File is too large.");

	// 기존에 동일한 이름의 파일이 있을 경우 삭제
	if (exists != FILEINFO_NULL) {
		ASSERT(Custom_FileSystem_Delete(exists));
	}

	// 마지막 파일 위치에 새로운 파일 작성
	FileInfo_t *lastFile = FILE(file);

	// 파일 길이 복사
	lastFile->size = length;

	// 파일이름 복사
	int i = 0;
	for (; filename[i]; i++) {
		lastFile->filename[i] = filename[i];
	}
	for (; i < FILENAME_LENGTH; i++) {
		lastFile->filename[i] = 0;
	}

	// 체크썸 계산
	lastFile->checksum = 0;
	lastFile->checksum = Custom_FileSystem_Get_CheckSum(lastFile);

	// 데이터 복사
	uint8_t *dst = DATA(lastFile);
	for (uint32_t i = 0; i < length; i++) {
		dst[i] = data[i];
	}

	return true;
}
