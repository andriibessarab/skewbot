import React, { useState } from "react";
import SkewbFace from "./DisplaySides";
import { GlobalAveragePixelValues } from "./PixelAnalyze";

function SaveColors() {
  const [faces, setFaces] = useState<number[][][]>([]);

  const handleClick = () => {
    if (faces.length < 6){
        setFaces([...faces, GlobalAveragePixelValues]);
    }
  };

  return (
    <div>
      <button onClick={handleClick}>Save Colors</button>
    <p style={{display:"flex", gap:"90px",textDecoration:"underline"}}>
    <span>Top</span>
    <span>Front</span>
    <span>Left</span>
    <span>Back</span>
    <span>Right</span>
    <span>Bottom</span>
    </p>
      <div style={{ display: "flex", gap: "15px", flexWrap: "wrap" }}>
        {faces.map((colors, key) => (
          <SkewbFace key = {key} Colors={colors} />
        ))}
      </div>
    </div>
  );
}

export default SaveColors;
