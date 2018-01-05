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
  
  let make = (~labels, ~onFilterChange, children) => {
    let onChange = (event, _self) => onFilterChange(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value);
    {
    ...component,
    render: (self) => {
        let labelOptions = ReasonReact.arrayToElement (
            Array.map(fun(label: issueLabel) => <option key=string_of_int(label.id)>{ReasonReact.stringToElement(label.name)}</option>, labels)
        );
        <div className=styles##filter>
            {ReasonReact.stringToElement("Filter")}
            <select className=styles##labels onChange={self.handle(onChange)}>
                {labelOptions}
            </select>
        </div>
        }
    }
  };