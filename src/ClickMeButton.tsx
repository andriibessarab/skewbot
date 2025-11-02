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
    <p style={{display:"flex", gap:"75px",textDecoration:"underline"}}>
    <span>Face1</span>
    <span>Face2</span>
    <span>Face3</span>
    <span>Face4</span>
    <span>Face5</span>
    <span>Face6</span>
    </p>
      <div style={{ display: "flex", gap: "10px", flexWrap: "wrap" }}>
        {faces.map((colors, key) => (
          <SkewbFace key = {key} Colors={colors} />
        ))}
      </div>
    </div>
  );
}

export default SaveColors;
