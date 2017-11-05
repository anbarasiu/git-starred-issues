/* open Bs_fetch; */
[%bs.raw {|require('./app.css')|}];
let component = ReasonReact.statelessComponent("App");


let make = (_children) => {
  let parseResponseJson = (json) =>
    Js.log("test");
  {
  ...component,
  didMount: (_self) => {
    Bs_fetch.fetch("https://api.github.com/users/anbarasiu/starred")
    |> Js.Promise.then_(Bs_fetch.Response.text)
    |> Js.Promise.then_(
      fun(jsonText) => {
        Js.log(jsonText);
        Js.Promise.resolve()
      }
    );
    ReasonReact.NoUpdate
  },
  render: (_self) =>
    <div className="App">
      <h2> (ReasonReact.stringToElement("Issues from Starred Repos")) </h2>
      <div>
        /* <LoginButton /> */
        <List>
          <ListItem />
        </List>
      </div>
    </div>
}
};
