import * as T from 'three';
const mat=(color,metalness=0,roughness=.6)=>new T.MeshStandardMaterial({color,metalness,roughness});
const carbon=mat('#151a21',.25,.38),metal=mat('#c1c8ce',.75,.25),green=mat('#216458'),black=mat('#222a31'),gold=mat('#bc9c55',.7,.25);
function box(parent,x,y,z,w,h,d,m){const a=new T.Mesh(new T.BoxGeometry(w,h,d),m);a.position.set(x,y,z);a.castShadow=true;parent.add(a);return a;}
function rod(parent,points,radius=.0007,m=carbon){const c=new T.CatmullRomCurve3(points.map(p=>new T.Vector3(...p)));const mesh=new T.Mesh(new T.TubeGeometry(c,Math.max(12,points.length*6),radius,5,false),m);mesh.castShadow=true;parent.add(mesh);return mesh;}
function wing(){
 const g=new T.Group();const outline=[[0,0],[.11,.022],[.188,.085],[.18,.18],[.105,.245],[.025,.221],[-.018,.166],[-.085,.202],[-.163,.152],[-.17,.075],[-.095,.025],[0,0]];
 const curve=new T.CatmullRomCurve3(outline.map(([x,z])=>new T.Vector3(x,0,z)),true,'centripetal');const points=curve.getPoints(150);
 const shape=new T.Shape(points.map(p=>new T.Vector2(p.x,p.z)));const geom=new T.ShapeGeometry(shape,24);geom.rotateX(Math.PI/2);
 // Shape's +y becomes +z, flat at y=0.
 const membrane=new T.Mesh(geom,new T.MeshPhysicalMaterial({color:'#f6b6cd',side:T.DoubleSide,transparent:true,opacity:.66,roughness:.55,metalness:0,depthWrite:false}));
 g.add(membrane);rod(g,points.map(p=>[p.x,0,p.z]),.0008);
 const root=[-.015,.001,.012];
 for(const i of [14,29,44,59,74,89,104,119,134]){const p=points[i];rod(g,[root,[(p.x+root[0])*.52,.003,p.z*.48],[p.x,0,p.z]],.00055);}
 rod(g,[[.01,.001,.012],[.06,.003,.09],[.105,0,.245]],.001);
 rod(g,[[0,.001,.01],[-.06,.003,.07],[-.163,0,.152]],.0008);
 return g;
}
export function makeButterfly(){
 const root=new T.Group();const left=new T.Group(),right=new T.Group();
 left.position.set(0,.018,-.022);right.position.set(0,.018,.022);
 const lw=wing();lw.scale.z=-1;left.add(lw);right.add(wing());root.add(left,right);
 // Frame, two servos, output pivots, horn hubs and supplied screws.
 box(root,0,0,0,.065,.0025,.052,carbon);
 for(const side of [-1,1]){
  box(root,-.004,.009,side*.018,.023,.019,.012,black);
  box(root,-.004,.019,side*.018,.027,.002,.014,black);
  for(const x of [-.014,.006]){const s=new T.Mesh(new T.CylinderGeometry(.0014,.0014,.002,10),metal);s.position.set(x,.021,side*.018);root.add(s);}
  const joint=side<0?left:right;
  const hub=new T.Mesh(new T.CylinderGeometry(.003,.003,.003,14),metal);hub.rotation.z=Math.PI/2;joint.add(hub);
  box(joint,.006,0,0,.014,.002,.003,mat('#e5e5df'));
  rod(root,[[-.012,.006,side*.02],[-.026,-.002,side*.018],[.007,.006,side*.003]],.0006,mat(side<0?'#b34644':'#c7b774'));
 }
 // Small perfboard/XIAO with USB-C and soldered pin rows.
 box(root,.008,.006,0,.030,.0016,.020,mat('#a77735'));
 box(root,.008,.010,0,.021,.001,.0175,green);
 box(root,.007,.012,0,.011,.002,.012,metal);
 box(root,.020,.012,0,.006,.003,.008,metal);
 for(let i=0;i<7;i++)for(const side of [-1,1])box(root,-.001+i*.00254,.011,side*.0085,.0011,.001,.0014,gold);
 box(root,-.004,-.009,0,.045,.012,.023,metal);
 for(const x of [-.018,.012])box(root,x,-.009,0,.003,.013,.024,black);
 box(root,.023,-.007,.009,.007,.005,.007,mat('#e4b649'));
 rod(root,[[.003,.013,0],[.010,.026,-.005],[.029,.033,-.009]],.0005,black);
 box(root,.035,.033,-.009,.018,.0006,.007,black);
 for(let i=0;i<4;i++)box(root,.029+i*.003,.0335,-.009,.001,.0001,.005,gold);
 return {root,left,right};
}
const qy=new T.Quaternion(),qz=new T.Quaternion(),qx=new T.Quaternion();
export function poseButterfly(b,s){
 if(!s)return;
 b.root.position.set(s[1],s[3],-s[2]);
 qy.setFromAxisAngle(new T.Vector3(0,1,0),s[5]);qz.setFromAxisAngle(new T.Vector3(0,0,1),s[7]);qx.setFromAxisAngle(new T.Vector3(1,0,0),s[6]);
 b.root.quaternion.copy(qy).multiply(qz).multiply(qx);
 b.left.rotation.x=s[8]*Math.PI/180;b.right.rotation.x=-s[9]*Math.PI/180;
}
