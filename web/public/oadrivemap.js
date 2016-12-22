/**
 * 
 */


function draw(stage,drawing,streetCont,objects,otherCar,checkpoint,fireCont, gameStatus) {
	if(drawing == true) {
		 return;
	 }	  
	 
	if( typeof gameStatus != 'undefined') {
		streetCont.removeAllChildren();
		var status_text;
		if(gameStatus == "winner") {
			status_text = new createjs.Text("You won :)", "50px Roboto", "green");
		} else {
			
	       status_text = new createjs.Text("You lost :(", "50px Roboto", "red");
	       
		}
		
		status_text.textAlign = "center"
       status_text.textBaseline = "middle";
       status_text.y = -100;
		
       stage.addChild(status_text);
		stage.update();
		stage.removeChild(status_text);
		
		return;
	}
	 
	 //make everthing clean
	streetCont.removeAllChildren();
	streetCont.x = 0;
	streetCont.y = 0;
	streetCont.regX = 0;
	streetCont.regY = 0;
	streetCont.rotation = 0;
	
	generateGrid(streetCont);

	 
	objects.patches.forEach(function(patch) {
			 
	 var x = patch.x * PIXEL_PER_METER;
	 var y = patch.y * PIXEL_PER_METER;
	 
	 var width;
	 var heigth;
	 
	 //oadrive definitions:
	 //enum PatchType { STRAIGHT=0, CROSS_SECTION=1, PARKING=2 };
	 //const float PATCH_WIDTHS[numPatchTypes] = { 1.0, 1.0, 0.47 };
	//const float PATCH_LENGTHS[numPatchTypes] = { 0.25, 1.0, 0.7853 };
	 switch(patch.patchType) {
	    case 0:
			width = 1.0 * PIXEL_PER_METER; 
			heigth = 0.25 * PIXEL_PER_METER;
	        break;
	    case 1:
	    	width = 1.0 * PIXEL_PER_METER; 
			heigth = 1.0 * PIXEL_PER_METER;
	        break;
	    case 2:
	    	width = 0.47 * PIXEL_PER_METER; 
			heigth = 0.7853 * PIXEL_PER_METER;
	        break;
	    default:
	    	width = 0; 
			heigth = 0;
		}
	 
	
	  var degree = rad2deg(patch.yaw);
	 
	  var rect = new createjs.Shape();
	  rect.graphics.beginStroke("blue").drawRect(0, 0, heigth, width);

	  rect.regX = heigth/2;
	  rect.regY = width/2;
	  rect.rotation = 360-degree;
	  
	  rect.x = x;
	  rect.y = y;
	  streetCont.addChild(rect);
			 
	});
	
	
	
	
	
	
	
	 
	
	//orientation to the car
	
	var carDeg = rad2deg(objects.car.yaw);
	var carX = objects.car.x * PIXEL_PER_METER;	
	var carY = objects.car.y * PIXEL_PER_METER;	
	
	var carRect = new createjs.Shape();
	carRect.graphics.beginStroke("green").drawRect(0, 0, 100, 50);

	carRect.regX = 100/2;
	carRect.regY = 50/2;
	carRect.rotation = 360-carDeg;
	carRect.x = carX;
	carRect.y = carY;
	

	//streetCont.addChild(carRect);
	
	
	streetCont.x = 0;
	streetCont.y = 0;
	streetCont.regX = carX;
	streetCont.regY = carY;
	streetCont.scaleY = -1;
	streetCont.rotation = -carDeg+270;
	
	
///////////////////
	/// Draw the other car
	
	if(typeof otherCar != 'undefined') {
		
		var alpha = 1.0;
		var circleCoordX = otherCar.x * PIXEL_PER_METER;
		var circleCoordY = otherCar.y * PIXEL_PER_METER;
		var lineEndX = otherCar.x * PIXEL_PER_METER;
		var lineEndY = otherCar.y * PIXEL_PER_METER;
		var angle = Math.atan2(otherCar.y - objects.car.y, otherCar.x - objects.car.x);
		
		var lineStartX =  (objects.car.x + 0.5 * Math.cos(angle)) * PIXEL_PER_METER;
		var lineStartY = ( objects.car.y + 0.5 * Math.sin(angle)) * PIXEL_PER_METER;
		
		//needed for hittest
		var stageRect = new createjs.Shape();
		stageRect.graphics.beginFill("green").drawRect(0, 0, 500, 500);
		stageRect.alpha = 0.0;
		stageRect.regX = 250;
		stageRect.regY = 250;
		stage.addChild(stageRect);
		  
		  
		
		var localCoord = streetCont.localToGlobal(circleCoordX, circleCoordY);

		
		if(stageRect.hitTest(localCoord.x, localCoord.y) == false) {
			alpha = 0.5;
			circleCoordX =  (objects.car.x + 0.8 * Math.cos(angle)) * PIXEL_PER_METER;
			circleCoordY = ( objects.car.y + 0.8 * Math.sin(angle)) * PIXEL_PER_METER;
			
			lineStartX =  (objects.car.x + 1.1 * Math.cos(angle)) * PIXEL_PER_METER;
			lineStartY = ( objects.car.y + 1.1 * Math.sin(angle)) * PIXEL_PER_METER;
			
			
		}
		
		var targetCont = new createjs.Container();
       var circleCont = new createjs.Container();
       
       targetCont.addChild(circleCont);
       
       
       var circle = new createjs.Shape();
       circle.graphics.beginFill("red").drawCircle(0, 0, 30);
       circle.alpha = alpha;
       circleCont.addChild(circle);
		
       
       
       targetCont.addChild(circleCont);
       
       
       targetCont.x = circleCoordX;
       targetCont.y =  circleCoordY;

       
       var textCont = new createjs.Container();
       var dist = otherCar.distance.toFixed(1).toString() + " m";
       var text = new createjs.Text(dist, "18px Roboto", "#ffffff");
       text.textAlign = "center"
        text.textBaseline = "middle";
       textCont.addChild(text);
       var bText = textCont.getBounds();
       textCont.regX = 0;
       textCont.regY = -1* (bText.height /4) + 4.5;
       textCont.scaleY = -1;
       textCont.rotation = 270;


       targetCont.addChild(textCont);
       
       var line = new createjs.Shape();
       line.graphics.setStrokeStyle(3);
       line.graphics.beginStroke("red");
       line.graphics.moveTo(lineStartX, lineStartY);
       line.graphics.lineTo(lineEndX, lineEndY);
       line.graphics.endStroke();
       line.alpha = alpha;
       streetCont.addChild(line);
       streetCont.addChild(targetCont);
	}
	
	
	
	
	/////
	
	///////////////////
	/// Draw checkpoint
	
	if(typeof checkpoint != 'undefined') {
		
		var cp_alpha = 1.0;
		var cp_circleCoordX = checkpoint.x * PIXEL_PER_METER;
		var cp_circleCoordY = checkpoint.y * PIXEL_PER_METER;
		var cp_lineEndX = checkpoint.x * PIXEL_PER_METER;
		var cp_lineEndY = checkpoint.y * PIXEL_PER_METER;
		var cp_angle = Math.atan2(checkpoint.y - objects.car.y, checkpoint.x - objects.car.x);
		
		var cp_lineStartX =  (objects.car.x + 0.5 * Math.cos(angle)) * PIXEL_PER_METER;
		var cp_lineStartY = ( objects.car.y + 0.5 * Math.sin(angle)) * PIXEL_PER_METER;
		
		//needed for hittest
		var cp_stageRect = new createjs.Shape();
		cp_stageRect.graphics.beginFill("green").drawRect(0, 0, 500, 500);
		cp_stageRect.alpha = 0.0;
		cp_stageRect.regX = 250;
		cp_stageRect.regY = 250;
		stage.addChild(cp_stageRect);
		  
		  
		
		var cp_localCoord = streetCont.localToGlobal(cp_circleCoordX, cp_circleCoordY);

		
		if(cp_stageRect.hitTest(cp_localCoord.x, cp_localCoord.y) == false) {
			cp_alpha = 0.5;
     deltaX = 0.35;
			cp_circleCoordX =  (deltaX + (objects.car.x + (deltaX + 0.8) * Math.cos(cp_angle))) * PIXEL_PER_METER;
			cp_circleCoordY = ( objects.car.y + (deltaX + 0.8) * Math.sin(cp_angle)) * PIXEL_PER_METER;
			
			cp_lineStartX =  (deltaX + (objects.car.x + (deltaX + 1.1) * Math.cos(cp_angle))) * PIXEL_PER_METER;
			cp_lineStartY = ( objects.car.y + (deltaX + 1.1) * Math.sin(cp_angle)) * PIXEL_PER_METER;
			
			
		}
		
		var cp_targetCont = new createjs.Container();
       var cp_circleCont = new createjs.Container();
       
       cp_targetCont.addChild(cp_circleCont);
       
       var textColor = "#ffffff";
       
       var cp_circle = new createjs.Shape();
       if(checkpoint.activated) {
       	cp_circle.graphics.beginFill("green").drawCircle(0, 0, 30);

       } else {
       	cp_circle.graphics.setStrokeStyle(3);
       	textColor = "#000000"
         cp_circle.graphics.beginFill("white").drawCircle(0, 0, 30);
       	cp_circle.graphics.beginStroke("green").drawCircle(0, 0, 30);
       }
       
       cp_circle.alpha = cp_alpha;
       cp_circleCont.addChild(cp_circle);
		
       
       
       cp_targetCont.addChild(cp_circleCont);
       
       
       cp_targetCont.x = cp_circleCoordX;
       cp_targetCont.y =  cp_circleCoordY;

       
       var cp_textCont = new createjs.Container();
       var cp_dist = checkpoint.distance.toFixed(1).toString() + " m";
       var cp_text = new createjs.Text(cp_dist, "18px Roboto", textColor);
       cp_text.textAlign = "center"
       cp_text.textBaseline = "middle";
       cp_textCont.addChild(cp_text);
       var cp_bText = cp_textCont.getBounds();
       cp_textCont.regX = 0;
       cp_textCont.regY = -1* (cp_bText.height /4) + 4.5;
       cp_textCont.scaleY = -1;
       cp_textCont.rotation = 270;


       cp_targetCont.addChild(cp_textCont);
       
       var cp_line = new createjs.Shape();
       cp_line.graphics.setStrokeStyle(3);
       cp_line.graphics.beginStroke("green");
       cp_line.graphics.moveTo(cp_lineStartX, cp_lineStartY);
       cp_line.graphics.lineTo(cp_lineEndX, cp_lineEndY);
       cp_line.graphics.endStroke();
       cp_line.alpha = cp_alpha;
       streetCont.addChild(cp_line);
       streetCont.addChild(cp_targetCont);
       
    
       var cp_text2 = new createjs.Text(checkpoint.total.toString() + "/2 checkpoints", "18px Roboto", "#fec400");
       cp_text2.textAlign = "right"
       cp_text2.textBaseline = "middle";
       cp_text2.x = 300;
       cp_text2.y = -300;
       stage.addChild(cp_text2);
       
       
	}
	
	
	
	
	/////
	
	
	stage.update();
	  
	  
	console.log("drawing finished");
	drawing = false;
}


function generateGrid(streetCont) {
	   
	   var length = PIXEL_PER_METER * 100;
	   var count = 50;
	   var start = -1* count;
	   //X
	   var i;
	   for(i=start; i < count; i++) {
		  	var rect = new createjs.Shape();
		  	
		  	if(i == 0) {
		  		//rect.graphics.beginFill("green").drawRect(0, 0, 4, length);
		  		rect.graphics.beginFill("grey").drawRect(0, 0, 4, length);
		  	} else {
		  		rect.graphics.beginFill("grey").drawRect(0, 0, 1, length);
		  	}
			

		  	rect.regY = length/2;
		  
		  	rect.x = i* PIXEL_PER_METER;
			rect.y = 0;
		
		  	streetCont.addChild(rect);
	   }
	   
	   var j;
	   for(j=start; j < count; j++) {
			var rect = new createjs.Shape();
			
			if(j == 0) {
				//rect.graphics.beginFill("red").drawRect(0, 0, length, 4);
				rect.graphics.beginFill("grey").drawRect(0, 0, length, 4);
			} else {
				rect.graphics.beginFill("grey").drawRect(0, 0, length, 1);
			}
			

			rect.regX = length/2;
		  
		  	rect.y = j* PIXEL_PER_METER;
			rect.x = 0;

		  streetCont.addChild(rect);
	   }
}


function rad2deg(radians) {
	   return -1.0 * radians * (180/Math.PI);
}