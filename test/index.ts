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
}


main().catch((e)=> console.error(e))