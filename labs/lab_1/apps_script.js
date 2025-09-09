function collectData() {
  var sheet = SpreadsheetApp.getActiveSheet();
  try {
    var response = UrlFetchApp.fetch(
      "https://api.particle.io/v1/devices/0a10aced202194944a06743c/temp?",
      {
        headers: {
          Authorization: "Bearer 8a2b7a6da0f10555a431700877dd7facc0f2bb1a",
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
