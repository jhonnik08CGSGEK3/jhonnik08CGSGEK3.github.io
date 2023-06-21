import { io } from "socket.io-client";
import { makeMove, white_flag } from "../dist/chessLogic.js"

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
    socket.on("MessageFromServer", function (msg, white_msg) {
      let res, capture;
      let textDoc = document.getElementById("history");
      let chatHist = document.getElementById("historyChat");
      console.log(msg);

      if (msg.length === 5 && msg[2] == ' ' && white_msg == white_flag) {
          let tableCell1 = document.getElementById(msg[0].toUpperCase() + msg[1]);
          let tableCell2 = document.getElementById(msg[3].toUpperCase() + msg[4]);
          if (tableCell2 !== null)
              if (tableCell2.innerHTML !== "")
                  capture = true;
          res = makeMove(tableCell1, tableCell2);
          if (res == -1) {
              if (white_msg) {
                  chatHist.value += "\n" + "White: " + msg;
                  document.getElementById("whoseTurn").innerText = "Black's turn";
              }
              else {
                  chatHist.value += "\n" + "Black: " + msg;
                  document.getElementById("whoseTurn").innerText = "White's turn";
              }
              chatHist.scrollTop = chatHist.scrollHeight;
          }
          else if (res == 0) {
              if (white_msg) textDoc.value += "\n" + "White: " + tableCell1.id + " " + tableCell2.id + " is an Invalid move!";
              else textDoc.value += "\n" + "Black: " + tableCell1.id + " " + tableCell2.id + " is an Invalid move!";
              errorAudio();
              textDoc.scrollTop = textDoc.scrollHeight;
          }
          else if (res == 1) {
              textDoc.value += "\n" + current_move + ". " + tableCell1.id + " " + tableCell2.id;
              textDoc.scrollTop = textDoc.scrollHeight;
              // Init sounds
              if (capture) captureAudio();
              else moveAudio();
          }
      }
      else {
          if (white_msg) chatHist.value += "\n" + "White: " + msg;
          else chatHist.value += "\n" + "Black: " + msg;
          chatHist.scrollTop = chatHist.scrollHeight;
      }
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
    else
        socket.emit("gameLeaveRequest");
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

  socket.on("startGame", (enemyID, white_flag) => {
    inMM = false;
    inGame = true;
    if (white_flag) {
        document.getElementById("whitePlayer").innerText = `White: ${socket.id} (You)`;
        document.getElementById("blackPlayer").innerText = `Black: ${enemyID}`;
    }
    else {
        document.getElementById("whitePlayer").innerText = `White: ${enemyID}`;
        document.getElementById("blackPlayer").innerText = `Black: ${socket.id} (You)`;
    }
    document.getElementById("joinButton").value = "Leave game";
    document.getElementById("whoseTurn").innerText = "White's turn";
    document.getElementById("chessBoard").innerHTML = getOriginalpos();
    notifyAudio();
  });

  socket.on("leftGame", () => {
    inMM = false;
    inGame = false;
    document.getElementById("whitePlayer").innerText = `White: Not in a match`;
    document.getElementById("blackPlayer").innerText = `Black: Not in a match`;
    document.getElementById("joinButton").value = "Join MM queue";
    document.getElementById("history").value = "";
    document.getElementById("historyChat").value = "";
    document.getElementById("whoseTurn").innerText = "";
    notifyAudio();
  });
}

window.addEventListener("load", (event) => {
  main();
});
