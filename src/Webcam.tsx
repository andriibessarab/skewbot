import { useEffect, useState, useRef} from "react";
import PixelAnalyze from "./PixelAnalyze";

function Webcam(){
    let [WebcamStatus, SetWebCamStatus] = useState<string>("WebcamOff");
    let canvasRef = useRef<HTMLCanvasElement>(null);
    const Dimensions: number[] = [1000,640];
    const video = useRef<HTMLVideoElement>(document.createElement("video"));
    const hasDrawnRef = useRef(false);
    const [OutputDivs, setOutputDivs] = useState<JSX.Element>();
    useEffect(()=>{
        async function GetWebCamPermissions(){
            try{
            const vid = await navigator.mediaDevices.getUserMedia({ video: true });
            video.current.srcObject = vid;
            SetWebCamStatus("WebcamOn");
            console.log(WebcamStatus);
            } catch(err){
            console.error("Webcam access denied or unavailable:", err);
        }
        }
       GetWebCamPermissions();
    },[]);
    useEffect(() => {
        video.current.addEventListener("canplay", ()=>{
            if (!hasDrawnRef.current){
                draw();
                video.current.play();
                console.log("apple");
            }
            hasDrawnRef.current = true;
             
        },{once:true});
      }, []);

    const draw = ()=>{
        const SkewbSize = 300;
        const CornerSideLength = 150;
        let canvas = canvasRef.current;
        let ctx = canvas?.getContext("2d", {willReadFrequently: true});
        
    
        if(ctx && canvas){
            //flip the mirrored image and also keeps original coordinate system
            ctx.save();
            ctx.translate(Dimensions[0], 0);  
            ctx.scale(-1, 1);     
            ctx.drawImage(video.current, 0, 0, Dimensions[0], Dimensions[1]);
            ctx.restore();
            //end
            const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
            let PixelsArray = imageData.data; 
            ctx.strokeStyle = "white";
            ctx.lineWidth = 5;
            const start = [(canvas.width-SkewbSize)/2,(canvas.height-SkewbSize)/2];
            ctx.beginPath();
            ctx.moveTo(start[0], start[1]);
            ctx.lineTo(start[0], start[1] + SkewbSize);
            ctx.lineTo(start[0] + SkewbSize, start[1] + SkewbSize);
            ctx.lineTo(start[0] + SkewbSize, start[1]);
            ctx.closePath();
            ctx.moveTo(start[0], start[1] + SkewbSize - CornerSideLength);
            ctx.lineTo(start[0] + CornerSideLength, start[1] + SkewbSize);
            ctx.moveTo(start[0] + SkewbSize - CornerSideLength, start[1] + SkewbSize);
            ctx.lineTo(start[0] + SkewbSize, start[1] + SkewbSize - CornerSideLength);
            ctx.moveTo(start[0] + SkewbSize, start[1] + CornerSideLength);
            ctx.lineTo(start[0] + SkewbSize - CornerSideLength, start[1]);
            ctx.moveTo(start[0] + CornerSideLength, start[1]);
            ctx.lineTo(start[0], start[1] + CornerSideLength);
            ctx.stroke();
            ctx.beginPath();
            
            let CenterPoints:number[][] = [[start[0] + SkewbSize / 2, start[1] + SkewbSize / 2],[start[0] + CornerSideLength / 3, start[1] + CornerSideLength / 3],[start[0] + SkewbSize - CornerSideLength / 3, start[1] + CornerSideLength / 3],[start[0] + SkewbSize - CornerSideLength / 3, start[1] + SkewbSize - CornerSideLength / 3],[start[0] + CornerSideLength / 3, start[1] + SkewbSize - CornerSideLength / 3]];
            for (let i = 0; i < CenterPoints.length; i++){
                ctx.strokeStyle = "rgba(255,0,0,0.2";
                ctx.beginPath();
                ctx.fillStyle = "rgba(255,0,0,0.2)";
                ctx.arc(CenterPoints[i][0],CenterPoints[i][1],10,0,2*Math.PI);
                ctx.fill();
                ctx.stroke();
            }
                setOutputDivs(PixelAnalyze(PixelsArray,CenterPoints,Dimensions));
                }
                requestAnimationFrame(draw);
                }
    
                return (
                    <div style={{ position: "relative", width: "70vw", height: "auto" }}>
                      <canvas
                        ref={canvasRef}
                        width={`${Dimensions[0]}px`}
                        height={`${Dimensions[1]}px`}
                        style={{ width: "100%", height: "auto", backgroundSize: "cover" }}
                      />
                      {OutputDivs}
                    </div>
                  );
                  
   
    }





export default Webcam;