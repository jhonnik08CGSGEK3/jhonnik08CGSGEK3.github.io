const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");

const app = express();
app.use(morgan("combined"));
app.use(express.static("."));

//initialize a simple http server
const server = http.createServer(app);
const io = new Server(server);

const clients = [];
const matchmaking = [];
const matches = [];

class Match {
  constructor(socket1, socket2) {
    this.whiteSocket = socket1;
    this.blackSocket = socket2;
    this.id = Date.now().toString();
  }
};

io.on("connection", (socket) => {
  clients.push(socket);
  //var room = uuid.v4();
  console.log(`Client connected with id: ${socket.id}`);
  socket.on("MessageToServer", (msg) => {
    let replyMsg = `${msg}`;
    console.log(replyMsg);
    for (let i = 0; i < matches.length; i++) {
      if (matches[i].whiteSocket == socket) { // white player message
        matches[i].whiteSocket.emit("MessageFromServer", replyMsg, true);
        matches[i].blackSocket.emit("MessageFromServer", replyMsg, true);
        break;
      }
      else if (matches[i].blackSocket == socket) { // black player message
        matches[i].whiteSocket.emit("MessageFromServer", replyMsg, false);
        matches[i].blackSocket.emit("MessageFromServer", replyMsg, false);
        break;
      }
    }
  });

  socket.on("disconnect", () => {
    console.log(`Client disconnected with id: ${socket.id}`);
    for (let i = 0; i < matches.length; i++) {
      if (matches[i].whiteSocket == socket || matches[i].blackSocket == socket) {
        matches[i].whiteSocket.emit("leftGame");
        matches[i].blackSocket.emit("leftGame");
        const index = matches.indexOf(matches[i]);
        if (index > -1) {
          matches.splice(index, 1);
        }
        break;
      }
    }
    const index1 = clients.indexOf(socket);
    if (index1 > -1) {
      clients.splice(index1, 1);
    }
    const index2 = matchmaking.indexOf(socket);
    if (index2 > -1) {
      matchmaking.splice(index2, 1);
    }
  });

  socket.on("mmJoinRequest", () => {
    console.log(`MATCHMAKING: Client joined MatchMaking with id: ${socket.id}`);
    socket.emit("joinedMM");
    matchmaking.push(socket);
    if (matchmaking.length >= 2) { // Join game request
      let socket1 = matchmaking[0];
      socket.emit("startGame", socket1.id, true);  // white player
      socket1.emit("startGame", socket.id, false); // black player
      console.log(`GAME: Clients ${socket.id} and ${socket1.id} started a match!`);
      let match = new Match(socket, socket1);
      matches.push(match);
      const index1 = matchmaking.indexOf(matchmaking[0]);
      if (index1 > -1) {
        matchmaking.splice(index1, 2);
      }
    }
  });
  
  socket.on("mmLeaveRequest", () => {
    console.log(`MATCHMAKING: Client LEFT MatchMaking with id: ${socket.id}`);
    socket.emit("leftMM");
    const index = matchmaking.indexOf(socket);
    if (index > -1) {
      matchmaking.splice(index, 1);
    }
  });

  socket.on("gameLeaveRequest", () => {
    for (let i = 0; i < matches.length; i++) {
      if (matches[i].whiteSocket == socket || matches[i].blackSocket == socket) {
        matches[i].whiteSocket.emit("leftGame");
        matches[i].blackSocket.emit("leftGame");
        const index = matches.indexOf(matches[i]);
        console.log(`GAME: Client ${socket.id} left his Match (ID: ${matches[i].id})`);
        if (index > -1) {
          matches.splice(index, 1);
        }
        break;
      }
    }
  });
});

server.listen(process.env.PORT || 1234, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});
