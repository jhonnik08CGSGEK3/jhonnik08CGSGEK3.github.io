import { io } from "socket.io-client";
import { makeMove } from "../dist/chessLogic.js"

function moveAudio() {
  var audio = new Audio();
  audio.src = 'bin/Move.mp3';
  audio.autoplay = true;
}
function captureAudio() {
  var audio = new Audio();
  audio.src = 'bin/Capture.mp3';
  audio.autoplay = true;
}
function errorAudio() {
  var audio = new Audio();
  audio.src = 'bin/Error.mp3';
  audio.autoplay = true;
}
function notifyAudio() {
  var audio = new Audio();
  audio.src = 'bin/GenericNotify.mp3';
  audio.autoplay = true;
}

let inGame = false, inMM = false;

async function main() {
  const socket = io();

  // client-side
  socket.on("connect", () => {
    console.log(socket.id); // x8WIv7-mJelg7on_ALbx
    //document.getElementById("whitePlayer").innerText = "White: " + socket.id + " (You)";
    socket.on("MessageFromServer", function (msg) {
        let res, capture;
        let textDoc = document.getElementById("history");
        let chatHist = document.getElementById("historyChat");
        console.log(msg);

        if (msg.length === 5 && msg[2] == ' ') {
            let tableCell1 = document.getElementById(msg[0].toUpperCase() + msg[1]);
            let tableCell2 = document.getElementById(msg[3].toUpperCase() + msg[4]);
            if (tableCell2 !== null)
                if (tableCell2.innerHTML !== "")
                    capture = true;
            res = makeMove(tableCell1, tableCell2);
            if (res == -1) {
                chatHist.value += "\n" + msg;
                chatHist.scrollTop = chatHist.scrollHeight;   
            }
            else if (res == 0) {
                textDoc.value += "\n" + tableCell1.id + " " + tableCell2.id + ": Invalid move!";
                errorAudio();
            }
            else if (res == 1) {
                textDoc.value += "\n" + current_move + ". " + tableCell1.id + " " + tableCell2.id;
                // Init sounds
                if (capture) captureAudio();
                else moveAudio();
            }
        }
        else {
            chatHist.value += "\n" + msg;
            chatHist.scrollTop = chatHist.scrollHeight; 
        }
        
        textDoc.scrollTop = textDoc.scrollHeight;
        //else return start.id + " " + finish.id + ": " + "Invalid move!";
    });
  });

  socket.on("disconnect", () => {
    console.log(socket.id); // undefined
  });

  document.getElementById("sentMsg").onkeyup = (ev) => {
    if (ev.code === "Enter" && inGame) {
      const value = document.getElementById("sentMsg").value;
      console.log(value);
      document.getElementById("sentMsg").value = "";

      socket.emit("MessageToServer", value);
    }
  };

  document.getElementById("joinButton").addEventListener("mousedown", () => {
    if (!inGame) {
      if (!inMM) socket.emit("mmJoinRequest");
      else socket.emit("mmLeaveRequest");
    }
      // document.getElementById("whitePlayer").innerText = `White: ${socket.id} (You)`;
  });

  socket.on("joinedMM", () => {
    inMM = true;
    document.getElementById("joinButton").value = "Leave MM queue";
  });

  socket.on("leftMM", () => {
    inMM = false;
    document.getElementById("joinButton").value = "Join MM queue";
  });

  socket.on("startGame", (blackID) => {
    inMM = false;
    inGame = true;
    document.getElementById("joinButton").value = "Leave game";
    document.getElementById("whitePlayer").value = `White: ${socket.id}`;
    document.getElementById("blackPlayer").value = `Black: ${blackID}`;
  });
  // document.getElementById("thePrompt").innerHTML = "You joined the game!";
  //           document.getElementById("enemyBoardName").innerHTML = `${enemyID} (Enemy)`;
  //           document.getElementById("theButton").value = "Concede";
  //           (inMM = false),
  //           (inGame = true);

//   document.getElementById("theButton").addEventListener("mousedown", ()=>{
//     if (!inGame) {
//         if (!inMM)
//             socket.emit("matchmakeRequest");
//         else
//             socket.emit("leaveMMrequest");
//     } else
//         socket.emit("concedeRequest");
// }
// );
}

window.addEventListener("load", (event) => {
  main();
});
