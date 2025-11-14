interface Props {
    Arr: number[][];
}
function DisplayColors(props: Props) {
    return (
        <>
            {props.Arr.map((avg, i) => (
                <div
                    key={i}
                    style={{
                        width: "30px",
                        height: "30px",
                        backgroundColor: `rgb(${avg[0]}, ${avg[1]}, ${avg[2]})`,
                        border: "2px solid black",
                    }}
                >
                    {" "}
                    <span style={{ position: "absolute", display: "inline-block", left: "35px" }}>
                        {Output(i)}
                    </span>{" "}
                </div>
            ))}
        </>
    );
}

function Output(i: number) {
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
export default DisplayColors;
