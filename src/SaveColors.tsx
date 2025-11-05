import React, { useState } from "react";
import SkewbFace, { GlobalSavedValues } from "./DisplaySides";
import { GlobalAveragePixelValues } from "./PixelAnalyze";



function SaveColors() {
  const [faces, setFaces] = useState<number[][][]>([]);

  const handleClick = () => {
    if (faces.length < 6){
        setFaces([...faces, GlobalAveragePixelValues]);
    }
  };
  const handleClearClick = () =>{
    setFaces([]);
   GlobalSavedValues.length = 0;
  }

  return (
    <div>
      <button onClick={handleClick}>Save Colors</button>
      <button onClick = {handleClearClick} style = {{marginLeft: "5vw"}}>Clear Faces</button>
    <p style={{display:"flex", gap:"90px",textDecoration:"underline"}}>
    <span>Top</span>
    <span>Front</span>
    <span>Left</span>
    <span>Back</span>
    <span>Right</span>
    <span>Bottom</span>
    </p>
      <div style={{ display: "flex", gap: "15px", flexWrap: "wrap" }}>
      {faces.map((colors, key) => {
          return <SkewbFace key={key} Colors={colors} FaceIndex={key}></SkewbFace>
        })}
      </div>
    </div>
  );
}

export default SaveColors;
