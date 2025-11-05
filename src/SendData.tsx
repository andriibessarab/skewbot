import { GlobalSavedValues } from "./DisplaySides"

function SendDataButton(){
    return (<button style={{position:"absolute",right:"50px", top:"50px", width:"10vw",height:"10vh"}}onClick={sendData}>Send Data!</button>)
}
function SendOverData(){
    console.log(GlobalSavedValues.flat().join(""));
}

// This is the correct client code, which now requires a server-side CORS fix.
async function sendData() {
    const url = "http://10.39.43.184:5001/api/send_state";
    const payload = {
        "state": GlobalSavedValues.flat().join("")
    };

    try {
        const response = await fetch(url, {
            method: "POST",
            headers: {
                "Content-Type": "application/json", // Server MUST have this
            },
            body: JSON.stringify(payload),
        });

        // ... (rest of the logic)
    } catch (error) {
        console.error("Error sending data:", error);
    }
}
  
export default SendDataButton