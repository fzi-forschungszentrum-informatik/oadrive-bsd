var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var serveStatic = require('serve-static')

const redis = require('redis');
const redisClient = redis.createClient();

app.use(serveStatic(__dirname + '/public', {'index': ['index.html']}))
//app.use(serveStatic(__dirname + '/public'))

//app.get('/', function(req, res){
//  res.sendFile(__dirname + '/html/index.html');
//});

var currentCar = 0;
var defaultVotingResult = 'straight';
var socketMapping = {};
var gameRunning = 1;
const turnVoteNeededPercentage = 0.5;
const boostVoteNeededPercentage = 0.3;
const turnVotesMaxNeeded = 10;
const boostVotesMaxNeeded = 10;

var emptyDirections = {
  left: 0,
  straight: 0,
  right: 0
  
};

var checkpoints = [];

var cars = [
  {
    commandChannel: 'oadrive/goffin/commands',
    statusChannel: 'oadrive/goffin/info/status',
    distanceChannel: 'oadrive/goffin/info/distance',
    mapChannel: 'oadrive/goffin/info/map',
    checkpointChannel: 'oadrive/goffin/info/checkpoint',
    name: 'goffin',
    displayName: 'Goffin',
    connected: false,
    votingResult: defaultVotingResult,
    votes: {},
    turnVotes: [],
    turnVotesNumber: 0,
    turnVotesNeeded: 10,
    boostVotes: [],
    boostVotesNumber: 0,
    boostVotesNeeded: 10,
    allVotes: [],
    allIntersections: [],
    allTurns: [],
    allBoosts: [],
    allMaps: [],
    directions: emptyDirections,
    distance: -1,
    numberClients: 0,
    carWon: -1,
    mode: 'hunt',
    lastDistanceUpdate: 0
  },
  {
    commandChannel: 'oadrive/inka/commands',
    statusChannel: 'oadrive/inka/info/status',
    distanceChannel: 'oadrive/inka/info/distance',
    mapChannel: 'oadrive/inka/info/map',
    checkpointChannel: 'oadrive/inka/info/checkpoint',
    name: 'inka',
    displayName: 'Inka',
    connected: false,
    votingResult: defaultVotingResult,
    votes: {},
    turnVotes: [],
    turnVotesNumber: 0,
    turnVotesNeeded: 1,
    boostVotes: [],
    boostVotesNumber: 0,
    boostVotesNeeded: 1,
    allVotes: [],
    allIntersections: [],
    allTurns: [],
    allBoosts: [],
    allMaps: [],
    directions: emptyDirections,
    distance: -1,
    numberClients: 0,
    carWon: -1,
    mode: 'escape',
    lastDistanceUpdate: 0
  }
];

var admins = [
];

var clients = [
];

function distance(aX,aY, bX, bY) {
	var a = bX - aX;
	var b = bY - aY;

	return Math.sqrt( a*a + b*b );
}

function currentTime() {
  return Math.floor(Date.now()/1000);
}

function log(socket, message) {
  console.log('* ' + socket.client.id + ' ' + message);
}

function getClientCar() {
  var clientCar = currentCar;
  currentCar = (currentCar + 1) % 2;
  return clientCar;
}

function calculateCompleteVote(car) {
  var votes = cars[car].votes;
  var directions = {
    left: 0,
    straight: 0,
    right: 0
  };

  for (var clientId in votes) {
    if (directions[votes[clientId].value] == undefined) {
      directions[votes[clientId].value] = 1;
    } else {
      directions[votes[clientId].value]++;
    }
  }
  cars[car].directions = directions;

  var maxVote = defaultVotingResult;
  var maxNum = 0;
  for (var direction in directions) {
    var currentNum = directions[direction];
    if (currentNum > maxNum) {
      maxVote = direction;
      maxNum = currentNum;
    }
  }
  return maxVote;
}

function switchTeams() {
  // copy elements of first car
  var car0commandChannel = cars[0].commandChannel;
  var car0statusChannel = cars[0].statusChannel;
  var car0distanceChannel = cars[0].distanceChannel;
  var car0mapChannel = cars[0].mapChannel;
  var car0checkpointChannel = cars[0].checkpointChannel;
  var car0name = cars[0].name;
  var car0displayName = cars[0].displayName;
  var car0mode = cars[0].mode;
  
  // switch users from car 1 to car 0
  cars[0].commandChannel = cars[1].commandChannel;
  cars[0].statusChannel = cars[1].statusChannel;
  cars[0].distanceChannel = cars[1].distanceChannel;
  cars[0].mapChannel = cars[1].mapChannel;
  cars[0].checkpointChannel = cars[1].checkpointChannel;
  cars[0].name = cars[1].name;
  cars[0].displayName = cars[1].displayName;
  cars[0].mode = cars[1].mode;
  
  // switch users from car 0 to car 1
  cars[1].commandChannel = car0commandChannel;
  cars[1].statusChannel = car0statusChannel;
  cars[1].distanceChannel = car0distanceChannel;
  cars[1].mapChannel = car0mapChannel;
  cars[1].checkpointChannel = car0checkpointChannel;
  cars[1].name = car0name;
  cars[1].displayName = car0displayName;
  cars[1].mode = car0mode;
  
  // notify users about changed cars
  for(var i = 0; i < 2; i++) {
    sendMessageToTeam('CAR', cars[i].displayName, i);
  }
  
  // update admins
  sendToAdmin();
}

function resetEnvironment() {
  checkpoints = [];
  
  for(var i = 0; i < 2; i++) {
    console.log("car " + i);
    var myCar = cars[i];
    myCar.votingResult = defaultVotingResult;
    myCar.votes = {};
    myCar.turnVotes = [];
    myCar.turnVotesNumber = 0;
    myCar.boostVotes = [];
    myCar.boostVotesNumber = 0;
    myCar.allVotes = [];
    myCar.allIntersections = [];
    myCar.allTurns = [];
    myCar.allBoosts = [];
    myCar.allMaps = [];
    myCar.directions = emptyDirections;
    myCar.distance = -1;
    myCar.carWon = -1;
    myCar.lastDistanceUpdate = 0;
    
    sendVoteToTeam(i);
    sendTurnToTeam(i);
    sendBoostToTeam(i);
  }
  
  sendToAdmin();
}

function sendToAdmin() {
  //console.log(cars);
  for (var i = 0; i < admins.length; i++) {
    log(admins[i], 'send current voting to admin');
    admins[i].emit('ADMIN', cars);
  }
}

function forwardToAdmin(channel, message, car) {
	  //we pack the original message in a packet which includes id of the car it was meant for
	var packed_message = {"car": car, "message": message};
	  for (var i = 0; i < admins.length; i++) {
	    admins[i].emit(channel, packed_message);
	  }
}

function sendMessageToTeam(channel, message, car) {
  var myCar = cars[car];
  for (var clientNumber in clients[car]) {
    var client = clients[car][clientNumber];
    var socket = socketMapping[client];
    log(socket, 'send a message to ' + client + ' through channel ' + channel);
    socket.emit(channel, message);
  }
  forwardToAdmin(channel, message, car);
}

function sendVoteToTeam(car) {
  var myCar = cars[car];
  sendMessageToTeam('VOTE', myCar.votingResult, car);
}

function sendDistanceToTeam(car) {
  var myCar = cars[car];
  //send to both!
  sendMessageToTeam('DISTANCE', myCar.distance, 0);
  sendMessageToTeam('DISTANCE', myCar.distance, 1);
}

function sendTurnToTeam(car) {
  var myCar = cars[car];
  console.log("sendTurnToTeam " + myCar.turnVotesNumber + ", car " + car);
  sendMessageToTeam('TURN', myCar.turnVotesNumber / myCar.turnVotesNeeded * 100, car);
}

function sendBoostToTeam(car) {
  var myCar = cars[car];
  sendMessageToTeam('BOOST', myCar.boostVotesNumber / myCar.boostVotesNeeded * 100, car);
}

function setDistance(car, distance) {
  cars[car].distance = distance;
  if(cars[car].lastDistanceUpdate + 1 < currentTime()) {
    cars[car].lastDistanceUpdate = currentTime();
    sendDistanceToTeam(car);
  }
  sendToAdmin();
}

function resetVotes(car) {
  //cars[car].allIntersections.push({time: currentTime(), direction: cars[car].votingResult});
  cars[car].votes = {};
  cars[car].votingResult = defaultVotingResult;
  calculateCompleteVote(car);
  sendVoteToTeam(car);
  sendDirectionToCar(car);
  sendToAdmin();
}

function sendDirectionToCar(car) {
  redisClient.publish(cars[car].commandChannel, cars[car].votingResult);
}

function updateCarVotes(clientCar) {
    var nextVote = calculateCompleteVote(clientCar);
    var myCar = cars[clientCar];
    if (myCar.votingResult != nextVote) {
      console.log('* car ' + clientCar + ' change voting result to ' + nextVote);
      myCar.votingResult = nextVote;
      sendDirectionToCar(clientCar);
      sendVoteToTeam(clientCar);
    }
    sendToAdmin();
}

function turnCar(clientCar) {
  cars[clientCar].allTurns.push({time: currentTime()});
  cars[clientCar].turnVotes = [];
  cars[clientCar].turnVotesNumber = 0;
  redisClient.publish(cars[clientCar].commandChannel, "turn");
  
  sendMessageToTeam('TURN', 1, clientCar);
  sendToAdmin();
  
  // send update to visualization after 3+1 seconds
  setTimeout(function() {
    sendToAdmin();
  }, 4000);
}

function boostCar(clientCar) {
  cars[clientCar].allBoosts.push({time: currentTime()});
  redisClient.publish(cars[clientCar].commandChannel, "speed_boost");
  
  sendMessageToTeam('BOOST', 1, clientCar);
  
  console.log("boost started for car " + cars[clientCar].displayName);
  
  // stop boost after 3 seconds
  setTimeout(stopBoostCar, 3000, clientCar);
  
//send update to visualization after 3+1 seconds
  setTimeout(function() {
    sendToAdmin();
  }, 4000);
}

function stopBoostCar(clientCar) {
  console.log("boost stopped for car " + cars[clientCar].displayName);
  cars[clientCar].boostVotes = [];
  cars[clientCar].boostVotesNumber = 0;
  redisClient.publish(cars[clientCar].commandChannel, "speed_normal");
}

function getGameResult(car) {
  var result = 'lost';
  if(cars[car].carWon == car) {
    result = 'won';
  }
  else if(cars[car].carWon == -1) {
    result = 'aborted';
  }
  
  return result;
}

function stopGame(carWon) {
  var notifyUsers = false;
  if(gameRunning == 1) {
    gameRunning = 0;
    notifyUsers = true;
  }
  for(car in cars) {
    cars[car].carWon = carWon;
    if(notifyUsers) {
      sendMessageToTeam('CONTROL', '{"command": "stop", "result": "' + getGameResult(car) + '"}', car);
      redisClient.publish(cars[car].commandChannel, "game_finished");
    }
  }
}

function startGame() {
  gameRunning = 1;
  for(car in cars) {
    // unlock buttons (if finished on crossing)
    sendMessageToTeam('ENTERED_CROSSING', 'entered_crossing', car);
    sendMessageToTeam('FINISHED_CROSSING', 'finished_crossing', car);
    
    // unlock interface
    sendMessageToTeam('CONTROL', '{"command": "start"}', car);
  }
  
  // send amount of checkpoints to admins
  for (var i = 0; i < admins.length; i++) {
    admins[i].emit("CHECKPOINT_NUMBER", checkpoints.length);
  }
}

//timer for boost and turn votes
setInterval(function(){
  var changed = false;
  
  for (var i = 0; i < cars.length; i++) {
    var time = Date.now();
    
    // delete old turn votes
    var numberTurnVotes = 0;
    var turnVotes = cars[i].turnVotes;
    for(var turnVote in turnVotes) {
      if(typeof turnVotes[turnVote] != 'undefined') {
        if(turnVotes[turnVote].time + 1000 < time) {
          cars[i].turnVotes.splice(numberTurnVotes, 1);
        }
        else {
          numberTurnVotes++;
        }
      }
    }
    if(cars[i].turnVotesNumber != numberTurnVotes) {
      cars[i].turnVotesNumber = numberTurnVotes;
      sendTurnToTeam(i);
      changed = true;
    }
    
     // delete old boost votes
    var numberBoostVotes = 0;
    var boostVotes = cars[i].boostVotes;
    for(var boostVote in boostVotes) {
      if(typeof boostVotes[boostVote] != 'undefined') {
        if(boostVotes[boostVote].time + 1000 < time) {
          cars[i].boostVotes.splice(numberBoostVotes, 1);
        }
        else {
          numberBoostVotes++;
        }
      }
    }
    if(cars[i].boostVotesNumber != numberBoostVotes) {
      cars[i].boostVotesNumber = numberBoostVotes;
      sendBoostToTeam(i);
      changed = true;
    }
  }
  if(changed) {
    sendToAdmin();
  }
}, 250);

//Handle incoming status info from car
function processRedisMessage(msg) {
  var messageProcessed = false;
  
  for( var i = 0; i < cars.length; i++) {
    if(msg.channel == cars[i].statusChannel) {
      messageProcessed = true;
      switch(msg.message) {
        case 'connected':
          console.log('car ' + cars[i].displayName + ' connected');
          cars[i].connected = true;
          // send current driving direction to car after connecting
          cars[i].votingResult = calculateCompleteVote(i);
          sendDirectionToCar(i);
          startGame();
          break;
        case 'disconnected':
          console.log('car ' + cars[i].displayName + ' disconnected');
          cars[i].connected = false;
          break;
        case 'reached_crossing':
          console.log('car ' + cars[i].displayName + ' reached crossing');          
          var crossing_result = {"winner": cars[i].votingResult, "votes": cars[i].directions}
          sendMessageToTeam('REACHED_CROSSING', crossing_result, i);
          resetVotes(i);
          break;
        case 'entered_crossing':
          console.log('car ' + cars[i].displayName + ' entered crossing');          
          sendMessageToTeam('ENTERED_CROSSING', 'entered_crossing', i);
          break;
        case 'finished_crossing':
          console.log('car ' + cars[i].displayName + ' finished crossing');          
          sendMessageToTeam('FINISHED_CROSSING', 'finished_crossing', i);
          break;
        case 'game_finished':
          console.log('game_finished signal received for car ' + cars[i].displayName);
        default:
          console.log('Unhandled status message ' + msg.message + ' from channel ' + msg.channel + ' received!');
          messageProcessed = false;
          break;
      }
    }
    else if(msg.channel == cars[i].distanceChannel) {
      console.log('car ' + cars[i].displayName + ' sent distance ' + msg.message);
      setDistance(i, msg.message);
      messageProcessed = true;
      
      var msgJson = JSON.parse(msg.message);
      if(cars[i].mode == 'hunt' && msgJson.distance < 1.5) {
    	  console.log(cars[i].displayName + ' wins! Game is over.');
    	  stopGame(i);
      } 
    }
    else if(msg.channel == cars[i].mapChannel) {
    	
      console.log('car ' + cars[i].displayName + ' map update.');
      
      sendMessageToTeam('MAP', msg.message, i);
      //cars[i].allMaps.push(msg.message);
      
      
    } else if(msg.channel == cars[i].checkpointChannel) {
    	   
    	var msgJson = JSON.parse(msg.message);
    	//checkpoints are only for the escaping one (inka)
    	if(cars[i].mode == 'escape') {
    		console.log('car ' + cars[i].displayName + ' checkpoint update.');
    		msgJson.activated = false;
    		//check if we now this checkpoint already
    		var j;
    		for(j=0; j < checkpoints.length; j++) {
    			
    			var c = checkpoints[j];
    			
    			//if they are in radius of 2.0 meters they are the same
    			if(distance(c.x,c.y, msgJson.x,msgJson.y) < 2.0 ) {
    				console.log('checkpoint was already activated.');
    				msgJson.activated = true;
    				msgJson.total = checkpoints.length;
    				sendMessageToTeam('CHECKPOINT', msgJson, i);
    				return messageProcessed;
    			} 			
    			
    		}
    		
			//if we are near enough we actived the checkpoint
			if(distance(msgJson.x,msgJson.y, msgJson.ownX, msgJson.ownY) < 2.0 ) {
				
				msgJson.activated = true;
				checkpoints.push(msgJson);
				console.log('new checkpoint activated! checkpoint' + checkpoints.length +'/2.');
				
				for (var k = 0; k < admins.length; k++) {
		      admins[k].emit("CHECKPOINT_NUMBER", checkpoints.length);
		    }
				
				if(checkpoints.length > 1) {
					console.log(cars[i].displayName + ' wins! Game is over');
					stopGame(i);
				}
			}
    		
			msgJson.total = checkpoints.length;
			
		  sendMessageToTeam('CHECKPOINT', msgJson, i);
		  return messageProcessed;
    	}
    }
  }
  
  return messageProcessed;
}

// redis stuff
const subscribe = redis.createClient();
subscribe.on("message", function(ch, m) {
  var redisPacket = {channel: ch, message: m}
  
  processRedisMessage(redisPacket);
});

for(car in cars) {
  subscribe.subscribe(cars[car].distanceChannel); //    listen to messages from distance-channel
  console.log('subscribed to channel ' + cars[car].distanceChannel);
  subscribe.subscribe(cars[car].statusChannel); //    listen to messages from status-channel
  console.log('subscribed to channel ' + cars[car].statusChannel);
  subscribe.subscribe(cars[car].mapChannel); //    listen to messages from map-channel
  console.log('subscribed to channel ' + cars[car].mapChannel);
  subscribe.subscribe(cars[car].checkpointChannel); //    listen to messages from checkpoint-channel
  console.log('subscribed to channel ' + cars[car].checkpointChannel);
}

// socket.io stuff
io.on('connection', function(socket) {
  var clientId = socket.client.id;
  var clientCar = undefined;
  socketMapping[clientId] = socket;
  log(socket, 'connected to server');

  // Register as admin
  socket.on('ADMIN', function () {
    log(socket, 'register as admin');
    admins[admins.length] = socket;
    sendToAdmin();
  });

  // Register as voter
  socket.on('CAR', function(info) {
    var car = info.car;
    for (var i = 0; i < cars.length; i++) {
      if (car == cars[i].name || car == cars[i].displayName) {
        clientCar = i;
      }
    }
    if (clientCar == undefined) {
      clientCar = getClientCar();
    }
    if(typeof clients[clientCar] == 'undefined') {
      clients[clientCar] = [];
    }
    clients[clientCar].push(clientId);
    cars[clientCar].numberClients++;
    cars[clientCar].boostVotesNeeded = Math.min(turnVotesMaxNeeded, Math.max(1, Math.ceil(cars[clientCar].numberClients * boostVoteNeededPercentage)));
    cars[clientCar].turnVotesNeeded = Math.min(boostVotesMaxNeeded, Math.max(1, Math.ceil(cars[clientCar].numberClients * turnVoteNeededPercentage)));
    log(socket, 'register car ' + clientCar);
    socket.emit('CAR', cars[clientCar].displayName);
    socket.emit('VOTE', cars[clientCar].votingResult);
    if(gameRunning == 0) {
      socket.emit('CONTROL', '{"command": "stop", "result": "' + getGameResult(clientCar) + '"}');
    }
  });

  // Handle incoming votes
  socket.on('VOTE', function(msg){
    var save = {time: currentTime(), value: msg.value, user: clientId};
    //cars[clientCar].allVotes.push(save);
    
    if (msg.value == 'turn') {
      log(socket, 'turn vote');
      save.time = Date.now();
      cars[clientCar].turnVotes.push(save);
      cars[clientCar].turnVotesNumber++;
      if(cars[clientCar].turnVotesNumber >= cars[clientCar].turnVotesNeeded) {
        turnCar(clientCar);
      }
      else {
        sendTurnToTeam(clientCar);
      }
      sendToAdmin();
    }
    else if (msg.value == 'boost') {
      log(socket, 'boost vote');
      save.time = Date.now();
      cars[clientCar].boostVotes.push(save);
      cars[clientCar].boostVotesNumber++;
      if(cars[clientCar].boostVotesNumber >= cars[clientCar].boostVotesNeeded) {
        boostCar(clientCar);
      }
      else {
        sendBoostToTeam(clientCar);
      }
      sendToAdmin();
    }
    else {
      log(socket, 'vote for ' + clientCar + ' with ' + msg.value);
      cars[clientCar].votes[clientId] = save;
      updateCarVotes(clientCar);
    }
  });
  
//Handle incoming votes
  socket.on('COMMAND', function(msg){
    if(msg == 'switchTeams') {
      log(socket, 'switching teams');
      switchTeams();
    }
    else if(msg == 'resetEnvironment') {
      log(socket, 'resetting environment');
      resetEnvironment();
    }
    else if(msg == 'startGame') {
      resetEnvironment();
      startGame();
    }
    else if(msg == 'stopGame') {
      stopGame(-1);
    }
  });

  socket.on('disconnect', function() {
    log(socket, 'disconnected from server');
    if (clientCar != undefined) {
      delete cars[clientCar].votes[clientId];
      updateCarVotes(clientCar);
      if(cars[clientCar].numberClients > 0) {
        cars[clientCar].numberClients--;
      }
      for(var clientNumber in clients[clientCar]) {
        if(clients[clientCar][clientNumber] == clientId) {
          clients[clientCar].splice(clientNumber, 1);
        }
      }

      cars[clientCar].boostVotesNeeded = Math.min(turnVotesMaxNeeded, Math.max(1, Math.ceil(cars[clientCar].numberClients * boostVoteNeededPercentage)));
      cars[clientCar].turnVotesNeeded = Math.min(boostVotesMaxNeeded, Math.max(1, Math.ceil(cars[clientCar].numberClients * turnVoteNeededPercentage)));
    }
  });
});

http.listen(80, function(){
  console.log('listening on *:80');
});


