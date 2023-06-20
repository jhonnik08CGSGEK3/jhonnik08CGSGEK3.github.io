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
const rooms = [];

io.on("connection", (socket) => {
  clients.push(socket);
  //var room = uuid.v4();
  console.log(`Client connected with id: ${socket.id}`);
  socket.on("MessageToServer", (msg) => {
    const replyMsg = `${msg}`;
    console.log(replyMsg);
    for (client of clients) {
      client.emit("MessageFromServer", replyMsg);
    }
  });
  socket.on("disconnect", () => {
    console.log(`Client disconnected with id: ${socket.id}`);
    const index = clients.indexOf(socket);
    if (index > -1) {
      clients.splice(index, 1);
    }
  });
  socket.on("mmJoinRequest", () => {
    console.log(`MATCHMAKING: Client joined MatchMaking with id: ${socket.id}`);
    socket.emit("joinedMM");
    matchmaking.push(socket);
    if (matchmaking.length >= 2) {
      let socket1 = matchmaking[0], socket2 = matchmaking[1];
      socket1.emit("startGame", socket2.id)
      socket2.emit("startGame", socket1.id)
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
});

server.listen(process.env.PORT || 1234, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});
