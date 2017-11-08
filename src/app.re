/* open Bs_fetch; */
open Json.Decode;

[%bs.raw {|require('./app.css')|}];

type repository = {
  id: int,
  name: string,
  full_name: string,
  html_url: string,
  open_issues_count: int
};

type repositories = array(repository);

let component = ReasonReact.statelessComponent("App");

let make = (_children) => {
  let parseResponseJson = (json) :repository => 
      {
        id: json |> field("id", int),
        name: json |> field("name", string),
        full_name: json |> field("full_name", string),
        html_url: json |> field("html_url", string),
        open_issues_count: json |> field("open_issues_count", int)
      };
  let parseArrayResponseJson = (json) :repositories => field("", array(parseResponseJson), json);
  {
  ...component,
  didMount: (_self) => {
    Services.getStarredRepos()
    |> Js.Promise.then_(Bs_fetch.Response.text)
    |> Js.Promise.then_(
      fun(jsonText) => {
      Js.Promise.resolve(parseArrayResponseJson(Js.Json.parseExn(jsonText)))
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
