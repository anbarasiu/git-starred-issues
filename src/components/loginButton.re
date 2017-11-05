let component = ReasonReact.statelessComponent("LoginButton");

let make = (_children) => {
  let onClick = (_event) => Js.log("test");
  {
  ...component,
  render: (_self) => <div> <button onClick=onClick>(ReasonReact.stringToElement("Login"))</button> </div>
  }
};