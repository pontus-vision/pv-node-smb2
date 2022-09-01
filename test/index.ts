import { PvDirent, PvNodeSmb2 } from '../lib/pv-smb2'



const main = async () => {
    const smb2Client = new PvNodeSmb2(
        process.env.PV_WORKGROUP||'workgroup', 
        process.env.PV_USERNAME ||'user', 
        process.env.PV_PASSWORD ||'password')

    const content = smb2Client.readFileSync(process.env.PV_SMB_URL||'smb://<host or IP>/Windows Path/goes/here/foo.txt')
    console.log(content.toString('base64'))

    const info = smb2Client.fileInfo(process.env.PV_SMB_URL||'smb://<host or IP>/Windows Path/goes/here/foo.txt')
    console.log (`${JSON.stringify(info)}`)
    
    const dataBuffer = Buffer.from("foo", "utf8");
    const write = smb2Client.writeFileSync(process.env.PV_SMB_URL||'smb://<host or IP>/Windows Path/goes/here/foo.txt', dataBuffer)
    console.log (write.toString())

    const oldPath = "smb://<host or IP>/Windows Path/goes/here/foo.txt"
    const newPath = "smb://<host or IP>/Windows Path/goes/here/bar.txt"
    const rename = smb2Client.renameFileSync(oldPath, newPath)
    console.log (rename.toString())
}


main().catch((e)=> console.error(e))