open Types;

let styles = Css.({
    "filter": style([
        fontWeight(Bold)
    ]),
    "labels": style([
        margin(px(8))
    ])
  });

  type state = {
      selectedFilter: string
  };
  
  let component = ReasonReact.statelessComponent("Filter");
  
  let make = (~labels, ~onFilterChange, children) => {
    let onChange = (_event, self) => onFilterChange(self.state.selectedFilter);
    {
    ...component,
    initialState: fun() => { selectedFilter: ""},
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