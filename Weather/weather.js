const express = require("express");
const axios = require("axios");
const app = express();
const port = 12345;

const url =
  "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getUltraSrtNcst";
const serviceKey =
  "kp4ewj5kRtcmwnft3uA36eP/xyEHJ5FC2WAT7YgfZyahrt5Ccqbmw1B4MuTQvPNRzA0JkWGF3uLwuKW4XJLnww==";

app.get("/", async (req, res, next) => {
  try{
    const today = new Date();
    const yyyy = today.getFullYear();
    const mm = String(today.getMonth() + 1).padStart(2, "0"); // 월은 0부터 시작하므로 1을 더해줍니다.
    const dd = String(today.getDate()).padStart(2, "0");
    const hours = String(today.getHours() - 2).padStart(2, "0");
    const minutes = String(today.getMinutes()).padStart(2, "0");

    const usValue = req.query.us; // 쿼리 파라미터(us)
    if (usValue) {
      console.log('ultrasonic sensor value:', usValue);
    } else {
      console.log('no signal');
    }

    const ret = await axios.get(url, {
      params: {
        serviceKey,
        pageNo: 1,
        numOfRows: 1000,
        dataType: "JSON",
        base_date: `${yyyy}${mm}${dd}`,
        base_time: `${hours}${minutes}`,
        nx: 55,
        ny: 127,
      },
    });

    const items = ret.data.response.body.items.item;
    const pty = items.filter((item) => item.category === "PTY");

    const categoryPTY = {
      0: 0,
      1: 1,
      2: 1,
      3: 1,
      4: 0,
      5: 1,
      6: 1,
      7: 0,
    };

    const htmlContent = `
    <!DOCTYPE html>
    <html>
    <head>
      <title>ultrasonicValue</title>
    </head>
    <body>
      <div id="message">ultrasonic</div>

      <script>
        const messageElement = document.getElementById('message');

        const newMessage = 
          "ultrasonic value: ${usValue}<br> rain constant: ${String(categoryPTY[pty[0].obsrValue])}";

        messageElement.innerHTML = newMessage;
      </script>
    </body>
    </html>

${String(categoryPTY[pty[0].obsrValue])}
  `;
  res.send(htmlContent);
  }catch(err){
    next(err);
  }
});

app.use((err, req, res, next) => {
  console.error(err.stack)
  res.status(500).send('error')
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

