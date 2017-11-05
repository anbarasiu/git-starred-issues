let component = ReasonReact.statelessComponent("List");

let make = (_children) => {
  {
  ...component,
  render: (_self) => <ul> </ul>
  }
};