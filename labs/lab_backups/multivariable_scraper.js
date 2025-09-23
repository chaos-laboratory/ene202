// == IMPORTANT ==
// make sure your device_id and token are correct!
var device_id = "YOUR_DEVICE_ID";
var token = "A_VALID_TOKEN";

// Enter all your data labels from your firmware in all_labels.
// E.g., if you have the lines:
//
//   Particle.variable("therm", thermistor);
//   Particle.variable("humid", humidity);
//
// all_labels should be ["term", "humid"].
var all_labels = ["LABEL_1", "LABEL_2", "LABEL_N"]; 

// == NO CHANGES BELOW HERE ==
// this function should be run on a timer-based trigger
function collectData() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var data = [] // this will hold our results
  for (const label of all_labels) {
    try {
      var individual_value = getVariable(label);
      data.push(individual_value);
    } catch (e) {
      Logger.log(e);
      data.push("error"); // saves an error message cell if there was an error
    }
  }

  // append the timestamp, data1, data... to the sheet
  var timestamp = new Date();
  sheet.appendRow([timestamp, ...data]); 
}

// encapsulates all fetching and parsing to retrieve the sensor values
function getVariable(label) {
  var response = UrlFetchApp.fetch(
    `https://api.particle.io/v1/devices/${device_id}/${label}?`,
    {
      headers: {
        Authorization: `Bearer ${token}`,
      },
    }
  );   ``
  var payload = JSON.parse(response.getContentText());
  var data = payload.result;
  return data;
}
