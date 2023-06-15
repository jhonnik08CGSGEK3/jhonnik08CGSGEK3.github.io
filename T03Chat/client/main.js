import { io } from "socket.io-client";
import { fs } from "fs";

async function main() {
  const socket = io();

  // client-side
  socket.on("connect", () => {
    console.log(socket.id); // http://192.168.30.40:6969/
    socket.on("MessageFromServer", function (msg) {
      let tag = document.getElementById("textBox");
      console.log(msg);
      let before = tag.value + "\n";
      console.log(`${socket.id} received message ${msg}`);
      tag.value = before + msg;
      tag.scrollTop = tag.scrollHeight;
    });
  });

  socket.on("disconnect", () => {
    console.log(socket.id); // undefined
  });

  document.getElementById("messageBox").onkeyup = (ev) => {
    if (ev.code === "Enter") {
      function isEmpty(str) {
        if (str.trim() == "") return true;
        return false;
      }
      let value = document.getElementById("messageBox").value;
      let nick = document.getElementById("nick").value;
      if (
        !isEmpty(value) &&
        !isEmpty(nick) &&
        nick.length <= 50 &&
        value.length <= 200
      ) {
        console.log(`message from client: ${value}`);
        document.getElementById("messageBox").value = "";
        value = nick + ": " + value;
        socket.emit("MessageToServer", value);
      }
    }
  };
}

window.addEventListener("load", (event) => {
  main();
});