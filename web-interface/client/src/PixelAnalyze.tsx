import DisplayColors from "./DisplayColors";

let GlobalAveragePixelValues: number[][] = [];

function rgbToHsv(r: number, g: number, b: number): [number, number, number] {
    r /= 255;
    g /= 255;
    b /= 255;

    const max = Math.max(r, g, b);
    const min = Math.min(r, g, b);
    const d = max - min;

    let h = 0;
    if (d === 0) {
        h = 0;
    } else if (max === r) {
        h = 60 * (((g - b) / d) % 6);
    } else if (max === g) {
        h = 60 * ((b - r) / d + 2);
    } else {
        h = 60 * ((r - g) / d + 4);
    }
    if (h < 0) h += 360;

    const s = max === 0 ? 0 : d / max;
    const v = max;

    return [h, s, v];
}

function classifyColorFromHSV(h: number, s: number, v: number): string {
    if (v > 0.7 && s < 0.35) return "White";
    if (h >= 0 && h < 10) return "Red";
    if (h >= 10 && h < 45) return "Orange";
    if (h >= 50 && h < 80) return "Yellow";
    if (h >= 80 && h < 160) return "Green";
    if (h >= 160 && h < 250) return "Blue";
    if (h >= 250 && h < 360) return "Red"; // Loop back to Red

    return "White"; // fallback
}

function PixelAnalyze(
    PixelArray: Uint8ClampedArray,
    PixelPoints: number[][],
    Dimensions: number[]
) {
    let AverageScanSideLength = 8; // For each PixelPoint needed to analyze, take the average around a square with length 10px
    let AveragePixelValues: number[][] = [];

    const DisplayThesePixelValues: { [key: string]: number[] } = {
        Red: [255, 0, 0],
        Orange: [255, 165, 0],
        Yellow: [255, 255, 0],
        Green: [0, 255, 0],
        Blue: [0, 0, 255],
        White: [255, 255, 255],
    };
    for (let i = 0; i < PixelPoints.length; i++) {
        let CurAverage: number[] = [0, 0, 0];
        for (let j = 0; j < AverageScanSideLength; j++) {
            for (let k = 0; k < AverageScanSideLength; k++) {
                let Point: number[] = GetRGB(
                    GetPixelIndex(
                        PixelPoints[i][0] - AverageScanSideLength / 2 + k,
                        PixelPoints[i][1] - AverageScanSideLength / 2 + j,
                        Dimensions
                    ),
                    PixelArray
                );
                for (let z = 0; z < CurAverage.length; z++) {
                    CurAverage[z] += Point[z] / (AverageScanSideLength * AverageScanSideLength);
                }
            }
        }
        //console.log(CurAverage);
        AveragePixelValues.push(CurAverage);
    }
    for (let i = 0; i < AveragePixelValues.length; i++) {
        const [r, g, b] = AveragePixelValues[i];

        const [h, s, v] = rgbToHsv(r, g, b);

        const name = classifyColorFromHSV(h, s, v);

        AveragePixelValues[i] = DisplayThesePixelValues[name];
    }

    GlobalAveragePixelValues = AveragePixelValues;

    return (
        <>
            <DisplayColors Arr={AveragePixelValues} />
        </>
    );
}

function GetRGB(index: number, PixelArray: Uint8ClampedArray) {
    return [PixelArray[index], PixelArray[index + 1], PixelArray[index + 2]];
}
function GetPixelIndex(x: number, y: number, Dimensions: number[]) {
    return (y * Dimensions[0] + x) * 4;
}
export default PixelAnalyze;
export { GlobalAveragePixelValues };
