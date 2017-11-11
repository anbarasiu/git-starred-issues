let component = ReasonReact.statelessComponent("ListItem");

let make = (~name, ~url, ~issues, _children) => {
  {
  ...component,
  render: (_self) => 
  <li>
    <div>(ReasonReact.stringToElement(name))</div>
    <div>(ReasonReact.stringToElement(url))</div>
    <div>(ReasonReact.stringToElement(string_of_int(issues)))</div>
  </li>
  }
};