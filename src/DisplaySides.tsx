import React from "react";
import "./DisplaySides.css";


interface Props{
    Colors:number[][]
}
function SkewbFace(props:Props) {
    const toRgba = (color: number[]) =>
        `rgba(${color[0]}, ${color[1]}, ${color[2]}, ${color[3]})`;
    
    //Had to switch around the orders since I flipped the webcam so if preserving order 0,1,2,3,4 then left corner color is going to be swapped with right corner color relative
    //because what you show on the webcam is going to be opposite what you see facing the skewb

    
  return (
      <div className="square">
        <div className="CenterSquare" style={{backgroundColor: toRgba(props.Colors[0])}} ></div>
         <div className="TopLeftTriangle" style={{backgroundColor: toRgba(props.Colors[2])}}></div>
         <div className="TopRightTriangle" style={{backgroundColor: toRgba(props.Colors[1])}}></div>
        <div className="BottomRightTriangle" style={{backgroundColor: toRgba(props.Colors[4])}}></div>
        <div className="BottomLeftTriangle" style={{backgroundColor: toRgba(props.Colors[3])}}></div>
    </div>
  );
}

export default SkewbFace;
