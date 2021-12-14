import bindings from "bindings";

const pv = bindings('pv');

export interface PvDirentBase {
  size: number;
  uid: number;
  gid: number;
  btime_ts: number;
  mtime_ts: number;
  ctime_ts: number;
  name: string;
  isFile: boolean;
  isDir: boolean;
}

export interface PvDirent extends PvDirentBase{
  btime: Date;
  mtime: Date;
  ctime: Date;
}

export class PvNodeSmb2 {
  private ctx:any;

  constructor(workgroup:string, username:string, password:string) {
    this.createContext(workgroup,username,password)
  }

  createContext(workgroup:string, username:string, password:string){
    this.ctx = pv.pvCreateContext(workgroup,username,password)
  }

  deleteContext(){
    pv.pvDeleteContext(this.ctx)
  }


  readFileSync(smbPath:string): Buffer{
    return pv.pvReadFromContext(this.ctx,smbPath)
  }

  readDir(smbPath:string):PvDirent[]{
    const base: PvDirentBase[] = pv.pvDirReadFromContext(this.ctx,smbPath)
    if (!base){
      return []
    }
    return base.map((entry:PvDirentBase)=>{
      return {
        ...entry,
        btime: new Date(entry.btime_ts),
        mtime: new Date(entry.mtime_ts),
        ctime: new Date(entry.ctime_ts)
      } as PvDirent
    });

  }


}
