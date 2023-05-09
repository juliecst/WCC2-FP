//this code is crafted together from Beckys osc Receive documentation and the code I wrote for the group project;
// at 12:33 09.05 I asked ChatGPT "Can you debug this code?"

//This sets up OSC
const osc = require("osc");
const os = require("os");

//I make a new port with the adress corresponding to the sending port of the esp32 
const udpPort = new osc.UDPPort({
  localAddress: "0.0.0.0",
  localPort: 6574,
});

udpPort.on("message", (oscMessage) => {
  const address = oscMessage.address;
  const args = oscMessage.args;
  const value = args[0];
  
  //When the ball sends a signal thtat is is being interacted with and it is received, the audio play function is triggered. 
  if (address === "/sensorValue" && value > 0) {
    goRandomAudio();
  }
});


function goRandomAudio() {
  if (currentAudio) {
    currentAudio.pause();
  }
  
  const audioSources = [
    'audio/Bm.m4a',
    'audio/Br.m4a',
    'audio/c.mp3',
    'audio/t.m4a',
    'audio/L.m4a'
  ];
//the audio is being randomly selected like I did in the code for the group project. 
  const randomIndex = Math.floor(Math.random() * audioSources.length);
  const randomAudio = new Audio(audioSources[randomIndex]);

  randomAudio.play();
  currentAudio = randomAudio;
}

udpPort.on("error", (err) => {
  console.error(`Error in OSC listener: ${err.message}`);
});

udpPort.open();
