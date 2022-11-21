import UdpUtil from 'lm_udp'
 
 
 
async function send(sleepms) {
    
    const aUdpChannel = new UdpUtil('localhost',7767)

    aUdpChannel.onMessage((msg,rinfo)=>{
        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
    })

    let sendThese = ['one', 'two','three', 'four', 'five']

    sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{ 
        console.log(`send ${m} success!`)}) 
    )

    await new Promise(r => setTimeout(r, 2000));
}


