import * as T from 'three';
export function makeWorld(rows){
 const scene=new T.Scene();scene.background=new T.Color('#a8bbc0');scene.fog=new T.Fog('#a8bbc0',28,70);
 scene.add(new T.HemisphereLight('#e3f4ff','#647d68',2.1));
 const sun=new T.DirectionalLight('#fff4df',3);sun.position.set(4,12,7);sun.castShadow=true;sun.shadow.mapSize.set(2048,2048);sun.shadow.camera.left=-14;sun.shadow.camera.right=14;sun.shadow.camera.top=14;sun.shadow.camera.bottom=-14;scene.add(sun);
 const floor=new T.Mesh(new T.PlaneGeometry(70,70),new T.MeshStandardMaterial({color:'#bac8be',roughness:.95}));floor.rotation.x=-Math.PI/2;floor.receiveShadow=true;scene.add(floor);
 const grid=new T.GridHelper(36,36,'#77908c','#a0b4aa');grid.position.y=.003;scene.add(grid);
 const track=rows.filter((_,i)=>i%8===0).map(r=>new T.Vector3(r[1],r[3],-r[2]));
 const line=new T.Line(new T.BufferGeometry().setFromPoints(track),new T.LineBasicMaterial({color:'#c47a98',transparent:true,opacity:.8}));scene.add(line);
 const reference=[];for(let i=0;i<=200;i++){const a=i*Math.PI*2/200;reference.push(new T.Vector3(8*Math.cos(a),.012,-8*Math.sin(a)));}
 scene.add(new T.Line(new T.BufferGeometry().setFromPoints(reference),new T.LineDashedMaterial({color:'#587579',dashSize:.2,gapSize:.2})).computeLineDistances());
 // An open test hall, no audience; pillars/windows create depth cues.
 const structure=new T.MeshStandardMaterial({color:'#e0e8e7',roughness:.7});
 for(const x of [-15,15])for(const z of [-18,-9,0,9,18]){const p=new T.Mesh(new T.BoxGeometry(.25,8,.25),structure);p.position.set(x,4,z);scene.add(p);}
 for(const z of [-18,18]){const wall=new T.Mesh(new T.BoxGeometry(30,1.2,.2),structure);wall.position.set(0,.6,z);scene.add(wall);}
 return scene;
}
