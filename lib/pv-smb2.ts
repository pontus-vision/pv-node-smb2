import bindings from 'bindings';

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


  fileInfo(smbPath:string):PvDirent{
    return pv.pvFileInfoFromContext(this.ctx,smbPath);
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
  /**
   * Write buffer to samba share 
   * 
   * @remarks
   * If the file exists it will be overwritten 
   * 
   * @param smbPath - Smb uri of the file ex. 'smb://<host or IP>/Windows Path/goes/here/foo.txt'
   * @param data - Buffer to be written
   * @returns Size of buffer which was written to the share
   * 
   * @beta
   */
  writeFileSync(smbPath:string, data: Buffer): number{
    return pv.pvWriteToContext(this.ctx,smbPath,data)
  }
  /**
   * Rename or move a file on samba share 
   * 
   * @remarks
   * If the new file exists it will be overwritten 
   * 
   * @param oldSmbPath - Smb uri of the file to be moved or renamed, ex: 'smb://<host or IP>/Windows Path/goes/here/foo.txt'
   * @param newSmbPath - Smb uri of the new file, ex: 'smb://<host or IP>/Windows Path/goes/here/foo.txt'
   * @returns Number 0 if successful
   * 
   * @beta
   */

  renameFileSync(oldSmbPath:string, newSmbPath: string): number{
    return pv.pvRenameOnContext(this.ctx,oldSmbPath,newSmbPath)
  }
}
