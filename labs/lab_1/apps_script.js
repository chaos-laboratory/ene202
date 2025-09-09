function collectData() {
  var sheet = SpreadsheetApp.getActiveSheet();
  try {
    var device_id = "YOUR_DEVICE_ID";
    var token = "CLASS_TOKEN_HERE";
    var response = UrlFetchApp.fetch(
      `https://api.particle.io/v1/devices/${device_id}/temp?`,
      {
        headers: {
          Authorization: `Bearer ${token}`,
        },
      }
    );
    var payload = JSON.parse(response.getContentText()); // parse the JSON text
    var timestamp = new Date();
    var data = payload.result;
    sheet.appendRow([timestamp, data]); // append the date, data1, data2 to the sheet
  } catch (e) {
    Logger.log("Could not connect to Particle.io");
  }
}
