let component = ReasonReact.statelessComponent("List");

let make = (children) => {
  {
  ...component,
  render: (_self) => <ul> {ReasonReact.arrayToElement(children)} </ul>
  }
};