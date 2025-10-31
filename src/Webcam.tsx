import { useEffect, useState, useRef} from "react";

var PixelsArray: Uint8ClampedArray;
const WindowSize = [window.innerWidth,window.innerHeight];
var IsDraw = false;

function Webcam(){
    let [WebcamStatus, SetWebCamStatus] = useState<string>("WebcamOff");
    let canvasRef = useRef<HTMLCanvasElement>(null);
    let video = useRef<HTMLVideoElement>(document.createElement("video"));
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
            draw();
            video.current.play();
            console.log("apple");
             
        },{once:true});
      }, []);

    const draw = ()=>{
        const SkewbSize = 300;
        const CornerSideLength = 150;
        let canvas = canvasRef.current;
        let ctx = canvas?.getContext("2d", {willReadFrequently: true});
    
        if(ctx && canvas){
            ctx.drawImage(video.current,0,0,canvas.width,canvas.height);
            const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
            PixelsArray = imageData.data; 
            ctx.strokeStyle = "white";
            ctx.lineWidth = 5;
            const start = [(canvas.width-SkewbSize)/2,(canvas.height-SkewbSize)/2];
            ctx.beginPath();
            ctx.moveTo(start[0],start[1]);
            ctx.lineTo(start[0],start[1]+SkewbSize);
            ctx.lineTo(start[0]+SkewbSize, start[1]+SkewbSize);
            ctx.lineTo(start[0]+SkewbSize, start[1]);
            ctx.closePath();
            ctx.moveTo(start[0],start[1]+SkewbSize-CornerSideLength);
            ctx.lineTo(start[0]+CornerSideLength,start[1]+SkewbSize);
            ctx.moveTo(start[0]+SkewbSize-CornerSideLength,start[1]+SkewbSize);
            ctx.lineTo(start[0]+SkewbSize,start[1]+SkewbSize-CornerSideLength);
            ctx.moveTo(start[0]+SkewbSize,start[1]+CornerSideLength);
            ctx.lineTo(start[0]+SkewbSize-CornerSideLength,start[1]);
            ctx.moveTo(start[0]+CornerSideLength,start[1]);
            ctx.lineTo(start[0],start[1]+CornerSideLength);
            ctx.stroke();
            
            
        }
        requestAnimationFrame(draw);
    }
    
    

    
    
    
    return (<canvas ref = {canvasRef} width = "640px" height="640px" style={{ width: `640px`, height: `640px` }}
></canvas>);
   
    }






export default Webcam;