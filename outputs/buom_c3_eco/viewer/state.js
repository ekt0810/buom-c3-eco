export const state={running:true,time:0,speed:1,mode:'follow',duration:60,rows:[],sample:null};
export async function loadFlight(){
 const r=await fetch('../matlab/results/trajectory.json');if(!r.ok)throw new Error('Chưa có trajectory.json; chạy MATLAB run_all trước.');
 state.rows=await r.json();if(state.rows.length<2)throw new Error('Dữ liệu quá ngắn');
 state.duration=state.rows.at(-1)[0];sampleAt(0);
}
export function sampleAt(time){
 const rows=state.rows;if(!rows.length)return;
 let lo=0,hi=rows.length-1;while(hi-lo>1){const m=(lo+hi)>>1;if(rows[m][0]<=time)lo=m;else hi=m;}
 const a=rows[lo],b=rows[hi],u=Math.min(1,Math.max(0,(time-a[0])/(b[0]-a[0])));
 state.sample=a.map((v,i)=>v+(b[i]-v)*u);return state.sample;
}
export function advance(dt){if(state.running)state.time=(state.time+dt*state.speed)%state.duration;sampleAt(state.time);}
