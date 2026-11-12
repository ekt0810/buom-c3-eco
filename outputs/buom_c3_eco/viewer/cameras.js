import * as T from 'three';
import {OrbitControls} from 'three/addons/controls/OrbitControls.js';
export function makeCamera(renderer){
 const camera=new T.PerspectiveCamera(44,innerWidth/innerHeight,.01,120);
 const controls=new OrbitControls(camera,renderer.domElement);controls.enableDamping=true;controls.minDistance=.35;controls.maxDistance=40;
 camera.position.set(9,2.3,1);controls.target.set(8,1.5,0);
 return {camera,controls,previousMode:''};
}
export function updateCamera(c,state,subject,dt){
 if(state.mode==='follow'){
  const offset=new T.Vector3(-.60,.36,.70).applyQuaternion(subject.quaternion);
  const goal=subject.position.clone().add(offset);
  c.camera.position.lerp(goal,1-Math.exp(-5*dt));c.controls.target.copy(subject.position);c.controls.enabled=false;
 }else{
  c.controls.enabled=true;if(c.previousMode!=='overview'){c.camera.position.set(14,12,14);c.controls.target.set(0,1.5,0);}
 }
 c.previousMode=state.mode;c.controls.update();
}
