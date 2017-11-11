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

type state = {
  repositories: option(repositories)
};

let component = ReasonReact.statefulComponent("App");

let handleReposLoaded = (repos, _self) => {
  ReasonReact.Update({
    repositories: Some(repos)
  });
};

let make = (_children) => {
  let parseResponseJson = (json) :repository => 
      {
        id: json |> field("id", int),
        name: json |> field("name", string),
        full_name: json |> field("full_name", string),
        html_url: json |> field("html_url", string),
        open_issues_count: json |> field("open_issues_count", int)
      };
  let parseArrayResponseJson = (json) :repositories => field("items", array(parseResponseJson), json);
  let fetchRepos = () => {
    Services.getStarredRepos()
    |> Js.Promise.then_(Bs_fetch.Response.text)
    |> Js.Promise.then_(
      fun(jsonText) => {
        Js.Promise.resolve(parseArrayResponseJson(Js.Json.parseExn(jsonText)))
      }
    );
  };
  {
    ...component,
    initialState: fun() => {repositories: None},
    didMount: ({update}) => {
      fetchRepos()
      |> Js.Promise.then_ (fun(repos) => {
        update(handleReposLoaded, repos);
        Js.Promise.resolve ();
      });
      ReasonReact.NoUpdate
    },
    render: fun({state}) => {
      let repoItems = switch (state.repositories) {
        | Some(repos) => ReasonReact.arrayToElement (Array.map(
            (fun (repo: repository) => <ListItem key=repo.name name=repo.name url=repo.html_url issues=repo.open_issues_count />), repos)
          )
        | None => ReasonReact.stringToElement("Loading")
      };
      <div className="App">
        <h2> (ReasonReact.stringToElement("Issues from Starred Repos")) </h2>
        <div>
          /* <LoginButton /> */
          <List>
            {repoItems}
          </List>
        </div>
      </div>
    }
  }
};
