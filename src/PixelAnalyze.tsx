import React from "react";
import DisplayColors from "./DisplayColors";
function PixelAnalyze(PixelArray: Uint8ClampedArray,PixelPoints:number[][],Dimensions: number[]){
    let AverageScanSideLength = 10; // For each PixelPoint needed to analyze, take the average around a square with length 10px 
    let AveragePixelValues:number[][] = [];
    for (let i = 0; i < PixelPoints.length; i++){
        let CurAverage:number[] = [0,0,0,0];
        for (let j = 0; j < AverageScanSideLength; j++){
            for(let k = 0; k < AverageScanSideLength; k++){
                let Point:number[] = GetRGBA(GetPixelIndex(PixelPoints[i][0]-(AverageScanSideLength/2)+k,PixelPoints[i][1]-(AverageScanSideLength/2)+j,Dimensions),PixelArray);
                for (let z = 0; z < CurAverage.length; z++){
                    CurAverage[z] += Point[z]/(AverageScanSideLength*AverageScanSideLength);
                }
            }


        }
        AveragePixelValues.push(CurAverage);
    }
   
    return (
        <>
          <DisplayColors Arr={AveragePixelValues} />

        </>
      );
      

      
      
    
}

function Output(i:number){
    switch (i) {
      case 0:
        return ": Center ";
      case 1:
        return ": TopLeft";
      case 2:
        return ": TopRight";
      case 3:
        return ": BottomRight";
      case 4:
        return ": BottomLeft";
      default:
        return "";
    }
    
}
function GetRGBA(index:number,PixelArray:Uint8ClampedArray){
    return [PixelArray[index],PixelArray[index+1],PixelArray[index+2],PixelArray[index+3]]
}
function GetPixelIndex(x:number,y:number,Dimensions:number[]){
    return  (y*Dimensions[0]+x)*4;
    
}
export default PixelAnalyze;