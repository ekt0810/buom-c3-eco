import * as T from 'three';
import {state,loadFlight,advance} from './state.js';
import {makeButterfly,poseButterfly} from './character.js';
import {makeWorld} from './world.js';
import {makeCamera,updateCamera} from './cameras.js';
import {setupUI,updateUI,showError} from './ui.js';
try{
 await loadFlight();
 const renderer=new T.WebGLRenderer({antialias:true});renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.setSize(innerWidth,innerHeight);renderer.shadowMap.enabled=true;renderer.toneMapping=T.ACESFilmicToneMapping;
 document.getElementById('viewport').append(renderer.domElement);
 const scene=makeWorld(state.rows),butterfly=makeButterfly();scene.add(butterfly.root);
 const cameras=makeCamera(renderer);setupUI();const clock=new T.Clock();let uiTime=0;
 renderer.setAnimationLoop(()=>{const dt=Math.min(clock.getDelta(),.05);advance(dt);poseButterfly(butterfly,state.sample);updateCamera(cameras,state,butterfly.root,dt);renderer.render(scene,cameras.camera);uiTime+=dt;if(uiTime>.08){updateUI();uiTime=0;}});
 window.addEventListener('resize',()=>{renderer.setSize(innerWidth,innerHeight);cameras.camera.aspect=innerWidth/innerHeight;cameras.camera.updateProjectionMatrix();});
 window.__BUOM_TEST__={state,scene,renderer,butterfly};
}catch(e){console.error(e);showError(e);}
