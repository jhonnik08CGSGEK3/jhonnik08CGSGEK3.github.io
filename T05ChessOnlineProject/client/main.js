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

let inGame = false, inMM = false, name = "";
let firstMoveId;

async function main() {
  const socket = io();

  var DragManager = new function() {

    /**
     * составной объект для хранения информации о переносе:
     * {
     *   elem - элемент, на котором была зажата мышь
     *   avatar - аватар
     *   downX/downY - координаты, на которых был mousedown
     *   shiftX/shiftY - относительный сдвиг курсора от угла элемента
     * }
     */
    var dragObject = {};

    var self = this;

    function onMouseDown(e) {

        if (e.which != 1) return;
        if (!inGame) return;
    
        var elem = e.target.closest('.draggable');
        if (!elem) return;
    
        dragObject.elem = elem;
        dragObject.id = elem.parentNode.id;
        firstMoveId = elem.parentNode.id;
        // dragObject.elem.getAttribute('id')
    
        // запомним, что элемент нажат на текущих координатах pageX/pageY
        dragObject.downX = e.pageX;
        dragObject.downY = e.pageY;
    
        return false;
    }

    function onMouseMove(e) {
        if (!dragObject.elem) return; // элемент не зажат
    
        if (!dragObject.avatar) { // если перенос не начат...
            var moveX = e.pageX - dragObject.downX;
            var moveY = e.pageY - dragObject.downY;
    
            // если мышь передвинулась в нажатом состоянии недостаточно далеко
            // if (Math.abs(moveX) < 3 && Math.abs(moveY) < 3) {
            //   return;
            // }
    
            // начинаем перенос
            dragObject.avatar = createAvatar(e); // создать аватар
            if (!dragObject.avatar) { // отмена переноса, нельзя "захватить" за эту часть элемента
                dragObject = {};
                return;
            }
    
            // аватар создан успешно
            // создать вспомогательные свойства shiftX/shiftY
            var coords = getCoords(dragObject.avatar);
            dragObject.shiftX = dragObject.downX - coords.left;
            dragObject.shiftY = dragObject.downY - coords.top;
    
            startDrag(e); // отобразить начало переноса
        }
    
        // отобразить перенос объекта при каждом движении мыши
        dragObject.avatar.style.left = e.pageX - 45 + 'px';
        dragObject.avatar.style.top = e.pageY - 45 + 'px';
    
        return false;
    }

    function onMouseUp(e) {
    if (dragObject.avatar) { // если перенос идет
        finishDrag(e);
    }

    // перенос либо не начинался, либо завершился
    // в любом случае очистим "состояние переноса" dragObject
        dragObject = {};
    }

    function finishDrag(e) {
        var dropElem = findDroppable(e);

        if (!dropElem) {
            self.onDragCancel(dragObject);
        } else {
            self.onDragEnd(dragObject, dropElem);
        }
    }

    function createAvatar(e) {

    // запомнить старые свойства, чтобы вернуться к ним при отмене переноса
    var avatar = dragObject.elem;
    if (avatar != undefined) {
        var old = {
            parent: avatar.parentNode,
            nextSibling: avatar.nextSibling,
            position: avatar.position || '',
            left: avatar.left || '',
            top: avatar.top || '',
            zIndex: avatar.zIndex || ''
        };
    
        // функция для отмены переноса
        avatar.rollback = function() {
            old.parent.insertBefore(avatar, old.nextSibling);
            avatar.style.position = old.position;
            avatar.style.left = old.left;
            avatar.style.top = old.top;
            avatar.style.zIndex = old.zIndex
        };
    
        return avatar;
    }
}

function startDrag(e) {
    var avatar = dragObject.avatar;

    // инициировать начало переноса
    document.body.appendChild(avatar);
    avatar.style.zIndex = 9999;
    avatar.style.position = 'absolute';
}

function findDroppable(event) {
    // спрячем переносимый элемент
    dragObject.avatar.hidden = true;

    // получить самый вложенный элемент под курсором мыши
    var elem = document.elementFromPoint(event.clientX, event.clientY);

    // показать переносимый элемент обратно
    dragObject.avatar.hidden = false;

    if (elem == null) {
        // такое возможно, если курсор мыши "вылетел" за границу окна
        return null;
    }

    return elem.closest('.droppable');
}

    document.onmousemove = onMouseMove;
    document.onmouseup = onMouseUp;
    document.onmousedown = onMouseDown;
    document.ondragstart = function() {
        return false;
    };

    this.onDragEnd = function(dragObject, dropElem) {};
    this.onDragCancel = function(dragObject) {};
  };

  function getCoords(elem) { // кроме IE8-
      var box = elem.getBoundingClientRect();

      return {
          top: box.top + scrollY,
          left: box.left + scrollX
      }; 
  }

  DragManager.onDragCancel = function(dragObject) {
      dragObject.avatar.rollback();
  };

  DragManager.onDragEnd = function(dragObject, dropElem) {
      // dropElem.innerHTML = `<img class="draggable" src="` + dragObject.elem.getAttribute('src') + `" alt="" class="img">`;
      // dragObject.elem.style.display = 'none';
      let res = firstMoveId + " " + dropElem.id;
      console.log(res);
      dragObject.avatar.rollback();
      socket.emit("MessageToServer", res);
  };

  // client-side
  socket.on("connect", () => {
    console.log(socket.id); // x8WIv7-mJelg7on_ALbx
    // document.getElementById("whitePlayer").innerText = "White: " + socket.id + " (You)";
    socket.on("MessageFromServer", function (msg, white_msg) {
        let res, capture;
        let textDoc = document.getElementById("history");
        let chatHist = document.getElementById("historyChat");
        console.log(white_flag);

        if (msg.length === 5 && msg[2] == ' ' && white_msg == white_flag) {
            let tableCell1 = document.getElementById(msg[0] + msg[1]);
            let tableCell2 = document.getElementById(msg[3] + msg[4]);
            if (tableCell2 !== null)
                if (tableCell2.innerHTML !== "")
                    capture = true;
            res = makeMove(tableCell1, tableCell2);
            console.log(res);
            if (res == -1) {
                //if (white_msg) chatHist.value += "\n" + "White: " + msg;
                //else chatHist.value += "\n" + "Black: " + msg;
                //chatHist.scrollTop = chatHist.scrollHeight;
            }
            else if (res == 0) {
                // if (white_msg) textDoc.value += "\n" + "White: " + tableCell1.id + " " + tableCell2.id + " is an Invalid move!";
                // else textDoc.value += "\n" + "Black: " + tableCell1.id + " " + tableCell2.id + " is an Invalid move!";
                errorAudio();
                // textDoc.scrollTop = textDoc.scrollHeight;
            }
            else if (res == 1) {
                if (white_msg) {
                    textDoc.value += "\n" + current_move + ". " + tableCell1.id + " " + tableCell2.id;
                    document.getElementById("whoseTurn").innerText = "Black's turn";
                }
                else {
                    textDoc.value += " | " + tableCell1.id + " " + tableCell2.id;
                    document.getElementById("whoseTurn").innerText = "White's turn";
                }
                textDoc.scrollTop = textDoc.scrollHeight;
                // Init sounds
                if (capture) captureAudio();
                else moveAudio();
            }
            else if (res == 2 || res == 3) {
                if (white_msg) {
                    textDoc.value += "\n" + current_move + ". " + tableCell1.id + " " + tableCell2.id;
                    //document.getElementById("whoseTurn").innerText = "White won!";
                }
                else {
                    textDoc.value += " | " + tableCell1.id + " " + tableCell2.id;
                    //document.getElementById("whoseTurn").innerText = "Black won!";
                }
                // notifyAudio();
                // white_flag = true;
                socket.emit("gameWinRequest");
            }
        }
        else if (msg.length != 5) {
            if (msg[2] != ' ') {
                if (white_msg) chatHist.value += "\n" + "White: " + msg;
                else chatHist.value += "\n" + "Black: " + msg;
                chatHist.scrollTop = chatHist.scrollHeight;
            }
        };
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

  //socket.on

  socket.on("startGame", (enemyID, white_flag) => {
    inMM = false;
    inGame = true;
    name = socket.id;
    // name = document.getElementById("nameInput").value;
    // if (name === "") name = "Anonymous";
    if (white_flag) {
        document.getElementById("whitePlayer").innerText = `White: ${name} (You)`;
        document.getElementById("blackPlayer").innerText = `Black: ${enemyID}`;
        document.getElementById("chessBoard").innerHTML = getOriginalpos(true);
    }
    else {
        document.getElementById("whitePlayer").innerText = `White: ${enemyID}`;
        document.getElementById("blackPlayer").innerText = `Black: ${name} (You)`;
        document.getElementById("chessBoard").innerHTML = getOriginalpos(false);
    }
    document.getElementById("joinButton").value = "Leave game";
    document.getElementById("whoseTurn").innerText = "White's turn";
    notifyAudio();
    initGame();
  });

  socket.on("leftGame", () => {
    inMM = false;
    inGame = false;
    document.getElementById("whitePlayer").innerText = `White: Not in a match`;
    document.getElementById("blackPlayer").innerText = `Black: Not in a match`;
    document.getElementById("joinButton").value = "Join MM queue";
    document.getElementById("history").value = "";
    document.getElementById("historyChat").value = "";
    document.getElementById("whoseTurn").innerText = "You lost!";
    notifyAudio();
  });

  socket.on("wonGame", (condition) => {
    inMM = false;
    inGame = false;
    document.getElementById("whitePlayer").innerText = `White: Not in a match`;
    document.getElementById("blackPlayer").innerText = `Black: Not in a match`;
    document.getElementById("joinButton").value = "Join MM queue";
    document.getElementById("history").value = "";
    document.getElementById("historyChat").value = "";
    notifyAudio();

    if (condition == undefined) document.getElementById("whoseTurn").innerText = "You won!";
    else if (condition == "opponentLeft") document.getElementById("whoseTurn").innerText = "You won! (Your opponent left the game)";
    else if (condition == "opponentDisconnected") document.getElementById("whoseTurn").innerText = "You won! (Your opponent disconnected)";
    else document.getElementById("whoseTurn").innerText = "You won!";
  });
}

window.addEventListener("load", (event) => {
  main();
});
