var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);

var light = {state: false};

app.use(express.static(__dirname + '/node_modules'));
app.get('/', function (req, res, next) {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function (client) {
  console.log('Client connected...');

  client.on('join', function (data) {
    console.log(data);
  });

  io.sockets.emit('led', light);
  client.on('toggle', function(state) {
    light.state = !light.state;
    console.log('id: ' + client.id + 'light: ' + light.state);
    io.sockets.emit('led', light);
  });

  client.on('disconnect', function () {
    console.log('Client disconnected!');
  });
});

server.listen(4200, () => {
  console.log('Server is up and running on port 4200')
});