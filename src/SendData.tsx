import { GlobalSavedValues } from "./DisplaySides"

function SendDataButton(){
    return (<button style={{position:"absolute",right:"50px", top:"50px", width:"10vw",height:"10vh"}}onClick={sendData}>Send Data!</button>)
}
function SendOverData(){
    console.log(GlobalSavedValues.flat().join(""));
}

async function sendData() {
    const url = "http://10.39.43.184:5001/api/send_state"; 
    const payload = {
        "state": GlobalSavedValues.flat().join("")
    };
  
    try {
      const response = await fetch(url, {
        method: "POST",          
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(payload),
      });
  
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
  
      const data = await response.json(); 
      console.log("Response from server:", data);
  
    } catch (error) {
      console.error("Error sending data:", error);
    }
  }
  
export default SendDataButton