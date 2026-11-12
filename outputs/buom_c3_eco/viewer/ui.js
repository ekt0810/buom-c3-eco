import {state,sampleAt} from './state.js';
const $=id=>document.getElementById(id);
export function setupUI(){
 const pause=()=>{state.running=!state.running;$('pause').textContent=state.running?'Tạm dừng':'Tiếp tục';};
 $('pause').onclick=pause;$('restart').onclick=()=>{state.time=0;sampleAt(0);};
 $('speed').onchange=e=>state.speed=Number(e.target.value);
 $('camera').onclick=()=>{state.mode=state.mode==='follow'?'overview':'follow';$('camera').textContent=state.mode==='follow'?'Toàn cảnh':'Bám bướm';};
 $('seek').max=state.duration;$('seek').oninput=e=>{state.time=Number(e.target.value);sampleAt(state.time);};
 window.addEventListener('keydown',e=>{if(e.code==='Space'&&e.target.tagName!=='INPUT'){e.preventDefault();pause();}});
 $('status').textContent='Dữ liệu từ run_all.m · WebGL / Three.js';
}
export function updateUI(){
 const s=state.sample;if(!s)return;
 $('seek').value=state.time;$('time').textContent=state.time.toFixed(1)+' s';$('height').textContent=s[3].toFixed(2)+' m';
 $('frequency').textContent=s[12].toFixed(2)+' Hz';$('angles').textContent=(s[6]*180/Math.PI).toFixed(1)+'° / '+(s[7]*180/Math.PI).toFixed(1)+'°';
 $('commands').textContent=s[10].toFixed(2)+' / '+s[11].toFixed(2);
}
export function showError(e){$('status').style.color='#ffc0b7';$('status').textContent=e.message;}
