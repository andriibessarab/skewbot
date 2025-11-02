import DisplayColors from "./DisplayColors";
var GlobalAveragePixelValues: number[][] = [];

function PixelAnalyze(PixelArray: Uint8ClampedArray,PixelPoints:number[][],Dimensions: number[]){
    let AverageScanSideLength = 8; // For each PixelPoint needed to analyze, take the average around a square with length 10px 
    let AveragePixelValues:number[][] = [];
    const KnownPixelValues: {[key:string]:number[]} = {
      "Red": [255, 0, 0],
      "Orange": [255, 165, 0],
      "Yellow": [255, 255, 0],
      "Green": [0, 255, 0],
      "Blue": [0, 0, 255],
      "White": [255, 255, 255]
    };
    for (let i = 0; i < PixelPoints.length; i++){
        let CurAverage:number[] = [0,0,0];
        for (let j = 0; j < AverageScanSideLength; j++){
            for(let k = 0; k < AverageScanSideLength; k++){
                let Point:number[] = GetRGB(GetPixelIndex(PixelPoints[i][0]-(AverageScanSideLength/2)+k,PixelPoints[i][1]-(AverageScanSideLength/2)+j,Dimensions),PixelArray);
                for (let z = 0; z < CurAverage.length; z++){
                    CurAverage[z] += Point[z]/(AverageScanSideLength*AverageScanSideLength);
                }
            }


        }
        AveragePixelValues.push(CurAverage);
    }
    for (let i = 0; i < AveragePixelValues.length; i++){
      let SmallestDiff = 1000000;
      let ValueOfDiff:number[] = [];
      for (let color in KnownPixelValues){
        let diff =
        Math.abs(AveragePixelValues[i][0] - KnownPixelValues[color][0]) +
        Math.abs(AveragePixelValues[i][1] - KnownPixelValues[color][1]) +
        Math.abs(AveragePixelValues[i][2] - KnownPixelValues[color][2]);
        if (diff < SmallestDiff){
          ValueOfDiff = KnownPixelValues[color];
          SmallestDiff = diff;
        }

      }
      AveragePixelValues[i] = ValueOfDiff;
      console.log(AveragePixelValues[i]);
    }


      GlobalAveragePixelValues = AveragePixelValues;
   
    return (
        <>
          <DisplayColors Arr={AveragePixelValues} />
        </>
      );
    
}

function GetRGB(index:number,PixelArray:Uint8ClampedArray){
    return [PixelArray[index],PixelArray[index+1],PixelArray[index+2]]
}
function GetPixelIndex(x:number,y:number,Dimensions:number[]){
    return  (y*Dimensions[0]+x)*4;
    
}
export default PixelAnalyze;
export {GlobalAveragePixelValues};