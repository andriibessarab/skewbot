import { useEffect, useState } from "react";
import Webcam from "./Webcam";
import SkewbFace from "./DisplaySides";
import SaveColors from "./ClickMeButton";


function App(){
 
  return (<div>Hello World <br /> <Webcam></Webcam> <SaveColors></SaveColors> </div>)
  
}

export default App;