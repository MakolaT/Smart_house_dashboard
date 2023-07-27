const gaugeElement = document.querySelector(".gauge");

function setGaugeValue(gauge, value) {
  if (value < 0 || value > 1) {
    return;
  }

  gauge.querySelector(".gauge__fill").style.transform = `rotate(${
    value / 2
  }turn)`;
  gauge.querySelector(".gauge__cover").textContent = `${Math.round(
    value * 100
  )}%`;
}

setGaugeValue(gaugeElement, 0.6);

function light1() {
  let lightswitch1 = document.getElementById("lightswitch1");
  let lightimg1 = document.getElementById("lightimg1");
  let lighton1 = document.getElementById("lighton1");

  if (lightswitch1.checked === true) {
    lightimg1.style.display = "none";
    lighton1.style.display = "block";
  } else {
    lightimg1.style.display = "block";
    lighton1.style.display = "none";
  }
}

function light2() {
  let lightswitch2 = document.getElementById("lightswitch2");
  let lightimg2 = document.getElementById("lightimg2");
  let lighton2 = document.getElementById("lighton2");

  if (lightswitch2.checked === true) {
    lightimg2.style.display = "none";
    lighton2.style.display = "block";
  } else {
    lightimg2.style.display = "block";
    lighton2.style.display = "none";
  }
}

function light3() {
  let lightswitch3 = document.getElementById("lightswitch3");
  let lightimg3 = document.getElementById("lightimg3");
  let lighton3 = document.getElementById("lighton3");

  if (lightswitch3.checked === true) {
    lightimg3.style.display = "none";
    lighton3.style.display = "block";
  } else {
    lightimg3.style.display = "block";
    lighton3.style.display = "none";
  }
}

function bedlamp1() {
  let bedlampswitch1 = document.getElementById("bedlampswitch1");
  let bedlampimg1 = document.getElementById("bedlampimg1");
  let bedlampon1 = document.getElementById("bedlampon1");

  if (bedlampswitch1.checked === true) {
    bedlampimg1.style.display = "none";
    bedlampon1.style.display = "block";
  } else {
    bedlampimg1.style.display = "block";
    bedlampon1.style.display = "none";
  }
}

let fanswitch2 = document.getElementById("fanswitch2");
let fan2 = document.getElementById("fan2");

let fanswitch3 = document.getElementById("fanswitch3");
let fan3 = document.getElementById("fan3");

let fanswitch4 = document.getElementById("fanswitch4");
let fan4 = document.getElementById("fan4");

function fanrotate1() {
  let fanswitch1 = document.getElementById("fanswitch1");
  let fan1 = document.getElementById("fan1");
  if (fanswitch1.checked === true) {
    fan1.className += " rotate";
  } else {
    fan1.className -= " rotate";
  }
}

function fanrotate2() {
  if (fanswitch2.checked === true) {
    fan2.className += " rotate";
  } else {
    fan2.className -= " rotate";
  }
}

function fanrotate3() {
  if (fanswitch3.checked === true) {
    fan3.className += " rotate";
  } else {
    fan3.className -= " rotate";
  }
}

function fanrotate4() {
  if (fanswitch4.checked === true) {
    fan4.className += " rotate";
  } else {
    fan4.className -= " rotate";
  }
}

function startTime() {
  let today = new Date();
  let day = today.getDate();
  let month = today.getMonth() + 1;
  let year = today.getFullYear();
  let h = today.getHours();
  let m = today.getMinutes();
  let s = today.getSeconds();
  m = checkTime(m);
  s = checkTime(s);
  let options = {
    hour: "numeric",
    minute: "numeric",
    hour12: true,
  };
  let timeString = today.toLocaleString("en-US", options);
  document.getElementById("date").innerHTML = day + "/" + month + "/" + year;
  // document.getElementById('time').innerHTML = h + ":" + m;
  document.getElementById("time").innerHTML = timeString;
  let t = setTimeout(function () {
    startTime();
  }, 500);

  var paragraph = document.getElementById("time").textContent;
  var searchPattern = /am/i; // i flag for case-insensitive search

  if (searchPattern.test(paragraph)) {
    document.getElementById("myParagraph").innerHTML = "Good Morning";
  } else {
    document.getElementById("myParagraph").innerHTML = "Good Afternoon";
  }
}

function checkTime(i) {
  if (i < 10) {
    i = "0" + i;
  } // add zero in front of numbers < 10
  return i;
}

// Switch the light on and off
function lightOnOff(id) {
  var image = document.getElementById(id);
  if (image.src.match("bulbon")) {
    image.src = "pic_bulboff.gif";
  } else {
    image.src = "pic_bulbon.gif";
  }
}

//Rotate the fan
function fanRotate(switchId, fanId) {
  let fanswitch = document.getElementById(switchId);
  let fan = document.getElementById(fanId);
  if (fanswitch.checked === true) {
    fan.className += " rotate";
  } else {
    fan.className -= " rotate";
  }
}
function navigateTo(url) {
  window.location.href = url;
}
function toggleLED(lightIndex) {
  console.log("Toggle LED function called");
  var checkbox = document.getElementById("lightswitch" + lightIndex);
  var state = checkbox.checked ? "on" : "off";
  var request = new XMLHttpRequest();
  request.open("GET", "/light?state=" + state, true);
  request.send();
}

// DOMContentLoaded event listener
document.addEventListener("DOMContentLoaded", function () {
  // JavaScript code goes here
  var checkbox = document.getElementById("lightswitch1");

  checkbox.addEventListener("change", function () {
    // Call the toggleLED function or add your logic here
    toggleLED(1);
  });
});

//relay function
function toggleRelay(relayNumber) {
  var relaySwitch = document.getElementById("lightswitch" + relayNumber);
  var relayImg = document.getElementById("lightimg" + relayNumber);

  if (relaySwitch.checked) {
    // Checkbox is checked, turn on the relay
    fetch("/relay" + relayNumber + "on")
      .then(function (response) {
        // Success message
        console.log("Relay " + relayNumber + " is ON");
        relayImg.src = "pic_bulbon.gif"; // Update the image to show the relay is on
      })
      .catch(function (error) {
        // Error message
        console.log("Error turning on Relay " + relayNumber + ": " + error);
      });
  } else {
    // Checkbox is not checked, turn off the relay
    fetch("/relay" + relayNumber + "off")
      .then(function (response) {
        // Success message
        console.log("Relay " + relayNumber + " is OFF");
        relayImg.src = "pic_bulboff.gif"; // Update the image to show the relay is off
      })
      .catch(function (error) {
        // Error message
        console.log("Error turning off Relay " + relayNumber + ": " + error);
      });
  }
}

function updateDashboard(snapshot) {
  var data = snapshot.val();
  var temperatureElement = document.getElementById("temperature");

  if (temperatureElement) {
    temperatureElement.innerText = "Temperature: " + data.temperature + " °C";
  }
}
