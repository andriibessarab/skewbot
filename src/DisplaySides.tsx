import React, { useState, useEffect } from "react";
import "./DisplaySides.css";

interface Props {
  Colors: number[][]; 
  FaceIndex: number;  
}


function IsEqual(arr1: number[], arr2: number[]) {
  return arr1.length === arr2.length && arr1.every((v, i) => v === arr2[i]);
}

function FindIndex(arr:number[], ColorCycle: number[][]){
  for (let i = 0; i < ColorCycle.length; i++){
    if (IsEqual(ColorCycle[i],arr)){
        return i;
    }
    
  }
  return 0;
}

var GlobalSavedValues: string[][] = [];

function SkewbFace({ Colors, FaceIndex }: Props) {
  const toRgb = (color: number[]) => `rgb(${color[0]}, ${color[1]}, ${color[2]})`;

  const ColorCycle: number[][] = [
    [255, 0, 0],
    [255, 165, 0],
    [255, 255, 0],
    [0, 255, 0],
    [0, 0, 255],
    [255, 255, 255],
  ];

  const ColorCycleString: string[] = ["R", "O", "Y", "G", "B", "W"];

  const [colors, setColors] = useState(Colors);


  useEffect(() => {
    GlobalSavedValues[FaceIndex] = [
      ColorCycleString[FindIndex(colors[0], ColorCycle)],
      ColorCycleString[FindIndex(colors[2], ColorCycle)],
      ColorCycleString[FindIndex(colors[1], ColorCycle)],
      ColorCycleString[FindIndex(colors[4], ColorCycle)],
      ColorCycleString[FindIndex(colors[3], ColorCycle)],
    ];
  }, [FaceIndex, colors]);
  

  const handleClick = (tileIndex: number) => {
    const newColors = [...colors];

    for (let i = 0; i < ColorCycle.length; i++) {
      if (IsEqual(colors[tileIndex], ColorCycle[i])) {
        const nextIndex = (i+1) % ColorCycle.length;
        newColors[tileIndex] = ColorCycle[nextIndex];
        GlobalSavedValues[FaceIndex][tileIndex] = ColorCycleString[nextIndex];
        break;
      }
    }

    setColors(newColors);
  };

  return (
    <div className="square">
      <div
        className="CenterSquare"
        style={{ backgroundColor: toRgb(colors[0]) }}
        onClick={() => handleClick(0)}
      />
      <div
        className="TopLeftTriangle"
        style={{ backgroundColor: toRgb(colors[2]) }}
        onClick={() => handleClick(2)}
      />
      <div
        className="TopRightTriangle"
        style={{ backgroundColor: toRgb(colors[1]) }}
        onClick={() => handleClick(1)}
      />
      <div
        className="BottomRightTriangle"
        style={{ backgroundColor: toRgb(colors[4]) }}
        onClick={() => handleClick(4)}
      />
      <div
        className="BottomLeftTriangle"
        style={{ backgroundColor: toRgb(colors[3]) }}
        onClick={() => handleClick(3)}
      />
    </div>
  );
}

export default SkewbFace;
export { GlobalSavedValues };
