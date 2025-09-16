function collectData() {
  var sheet = SpreadsheetApp.getActiveSheet();
  try {
    var device_id = "0a10aced202194944a06743c"; // Input your device ID
    var token = "af35f9f7bc71feaa9fff4cae9e6f8410594f7890"; // Input the token
    var label1 = "temp"; // Input the first variable name from your Particle code
    var label2 = "humid"; // Input the second variable name from your Particle code


    // This snippet now fetches only the data from the variable in your particle code that has been labeled 'label1', and saves it to the variable data1
    var response1 = UrlFetchApp.fetch(
      `https://api.particle.io/v1/devices/${device_id}/${label1}?`,
      {
        headers: {
          Authorization: `Bearer ${token}`,
        },
      }
    );
    var payload1 = JSON.parse(response1.getContentText()); // parse the JSON text
    var data1 = payload1.result;


    // This snippet now fetches only the data from the variable in your particle code that has been labeled 'label2', and saves it to the variable data2
    var response2 = UrlFetchApp.fetch(
      `https://api.particle.io/v1/devices/${device_id}/${label2}?`,
      {
        headers: {
          Authorization: `Bearer ${token}`,
        },
      }
    );
    var payload2 = JSON.parse(response2.getContentText()); // parse the JSON text
    var data2 = payload2.result;


    // This is where we collect our various data streams and get them to plot on the google sheet
    var timestamp = new Date();
    sheet.appendRow([timestamp, data1, data2]); // append the date, data1, data2 to the sheet
  } catch (e) {
    Logger.log("Could not connect to Particle.io. Check your device_id and token.");
  }
}