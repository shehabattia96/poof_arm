<script src="https://threejs.org/build/three.js"></script>
<script src="https://threejs.org/examples/js/controls/OrbitControls.js"></script>


// Init Scene and Rendered
var scene = new THREE.Scene();
var camera = new THREE.PerspectiveCamera( 100, window.innerWidth/window.innerHeight, 1, 10000 );
camera.position.z = 500;
//var renderer = new THREE.CanvasRenderer();
var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );


material = new THREE.MeshNormalMaterial();


// Create ground
var groundGeometry = new THREE.BoxGeometry( 400, 400, 10 );
var ground = new THREE.Mesh( groundGeometry, material );
ground.position.set( 0, 0, -100 );
scene.add( ground );

// Create ball
var ballGeometry = new THREE.SphereGeometry( 100, 32, 32 );
var ball = new THREE.Mesh( ballGeometry, material );

// Create shaft
var shaftGeometry = new THREE.BoxGeometry( 50, 50, 250 );
var shaft = new THREE.Mesh( shaftGeometry, material );
shaft.position.set( 0, 0, 100 );


// Group ball with shaft
var ballWithShaft = new THREE.Group();
ballWithShaft.add( ball );
ballWithShaft.add( shaft );
scene.add( ballWithShaft );
ballWithShaftRotationLimits = {x: {min: -1.44, max: 1.44},y: {min: -1.44, max: 1.44}}

renderer.render( scene, camera );
var controls = new THREE.OrbitControls( camera, renderer.domElement );
controls.addEventListener( 'change', ()=>{
				renderer.render( scene, camera );
        console.log('camera.rotation', camera.rotation)
} ); // add this only if there is no animation loop (requestAnimationFrame)


var rotateByValue = function (object, orientation, value, limitValue = null) {
	if (['x', 'y', 'z'].includes(orientation)) {
  	if (limitValue === null || Math.abs(object.rotation[orientation] + value) < Math.abs(limitValue)) {
      object.rotation[orientation] += value;
      renderer.render( scene, camera );
    }
  } else {
  	console.error('rotateByValue: invalid orientation', orientation)
  }
    console.log('Current rotation ', orientation, ': ',object.rotation[orientation])
};
var translateByValue = function (object, orientation, value) {
	if (['x', 'y', 'z'].includes(orientation)) {
    object.position[orientation] += value;
    renderer.render( scene, camera );
  } else {
  	console.error('translateByValue: invalid orientation', orientation)
  }
    console.log('Current position ', orientation, ': ',object.position[orientation])
};


var sensitivity = 0.006;
var keyMap = {65: 'A', 87: 'W', 68: 'D', 83: 'S'};

var pressedKeys = {'A': false, 'W': false, 'D': false, 'S': false, };
// from https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/key
document.body.addEventListener("keydown", function (event) {
    var key = keyMap[event.keyCode];
    pressedKeys[key] = true;
});

document.body.addEventListener("keyup", function (event) {
    var key = keyMap[event.keyCode];
    pressedKeys[key] = false;
});

function handleKeyPress() {
	requestAnimationFrame( handleKeyPress );
	if (pressedKeys["S"])
    	rotateByValue(ballWithShaft, 'x', sensitivity, ballWithShaftRotationLimits.x.max);
   
	if (pressedKeys["W"])
    	rotateByValue(ballWithShaft, 'x', -sensitivity,  ballWithShaftRotationLimits.x.min);
 
	if (pressedKeys["A"])
    		rotateByValue(ballWithShaft, 'y', -sensitivity,  ballWithShaftRotationLimits.y.min);
  
	if (pressedKeys["D"])
    		rotateByValue(ballWithShaft, 'y', sensitivity,  ballWithShaftRotationLimits.y.max);
}
handleKeyPress();