import { io } from "socket.io-client";
import { makeMove } from "../dist/chessLogic.js"

async function main() {
  const socket = io();

  // client-side
  socket.on("connect", () => {
    console.log(socket.id); // x8WIv7-mJelg7on_ALbx
    socket.on("MessageFromServer", function (msg) {
        let res;
        let textDoc = document.getElementById("history");
        console.log(msg);

        if (msg.length === 5 && msg[2] == ' ') {
            let tableCell1 = document.getElementById(msg[0] + msg[1]);
            let tableCell2 = document.getElementById(msg[3] + msg[4]);
            res = makeMove(tableCell1, tableCell2);
            //tableCell1.sty
            textDoc.value += "\n" + res;
        }
        else {
            textDoc.value += "\n" + msg;
        }
        
        textDoc.scrollTop = textDoc.scrollHeight;
    });
  });

  socket.on("disconnect", () => {
    console.log(socket.id); // undefined
  });

  document.getElementById("sentMsg").onkeyup = (ev) => {
    if (ev.code === "Enter") {
      const value = document.getElementById("sentMsg").value;
      console.log(value);
      document.getElementById("sentMsg").value = "";

      socket.emit("MessageToServer", value);
    }
  };
}

window.addEventListener("load", (event) => {
  main();
});
