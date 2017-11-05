let component = ReasonReact.statelessComponent("ListItem");

let make = (_children) => {
  {
  ...component,
  render: (_self) => <li> </li>
  }
};