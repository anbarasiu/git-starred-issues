open Types;

let styles = Css.({
    "filter": style([
        fontWeight(Bold)
    ]),
    "labels": style([
        margin(px(8))
    ])
  });
  
  let component = ReasonReact.statelessComponent("Filter");
  
  let make = (~labels, children) => {
    let onChange = (_event) => {
        Js.log("change");
    };
    {
    ...component,
    render: (self) => {
        let labelOptions = ReasonReact.arrayToElement (
            Array.map(fun(label: issueLabel) => <option>{ReasonReact.stringToElement(label.name)}</option>, labels)
        );
        <div className=styles##filter>
            {ReasonReact.stringToElement("Filter")}
            <select className=styles##labels onChange=onChange>
                {labelOptions}
            </select>
        </div>
        }
    }
  };