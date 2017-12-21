open Json.Decode;

[%bs.raw {|require('./app.css')|}];

type repository = {
  id: int,
  name: string,
  full_name: string,
  html_url: string,
  open_issues_count: int,
  url: string
};

type repositories = array(repository);

type issue = {
  id: int
};

type issues = array(issue);

type action = 
  | GetRepos(repositories)
  | GetRepoIssues(issues);

type state = {
  repositories: option(repositories),
  issues: option(issues)
};

let parseIssuesResponseJson = (json: Js.Json.t) :issue => 
{
  id: json |> field("id", int)
};
let parseIssuesArrayResponseJson = (json: Js.Json.t) :issues => array(parseIssuesResponseJson, json);

let fetchIssues = (issuesUrl) => {
  Js.log(issuesUrl);
  Services.getIssuesForRepo(issuesUrl)
  |> Js.Promise.then_(Bs_fetch.Response.text)
  |> Js.Promise.then_(
    fun(jsonText) => {
      Js.Promise.resolve(parseIssuesArrayResponseJson(Js.Json.parseExn(jsonText)))
    }
  );
};

let getRepoIssues = (repos, {ReasonReact.reduce: reduce}) => {
  Array.iter((repo: repository) => {
    let issuesUrl: string = repo.url;
    fetchIssues(issuesUrl)
    |> Js.Promise.then_ (fun(issues: issues) => {
      Js.log(issues);
      reduce(issues => GetRepoIssues(issues));
      Js.Promise.resolve ();
    });
    Js.log(issuesUrl);
  }, repos);
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  let parseResponseJson = (json: Js.Json.t) :repository => 
      {
        id: json |> field("id", int),
        name: json |> field("name", string),
        full_name: json |> field("full_name", string),
        html_url: json |> field("html_url", string),
        open_issues_count: json |> field("open_issues_count", int),
        url: json |> field("url", string)
      };
  let parseArrayResponseJson = (json: Js.Json.t) :repositories => array(parseResponseJson, json);
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
    initialState: fun() => {repositories: None, issues: None},
    didMount: (self) => {
      let handleReposLoaded = self.reduce(repositories => GetRepos(repositories));
      fetchRepos()
      |> Js.Promise.then_ (fun(repos) => {
        handleReposLoaded(repos);
        Js.Promise.resolve ();
      });
      ReasonReact.NoUpdate
    },
    reducer: fun(action, state) =>
      switch action {
      | GetRepos (repos) => ReasonReact.UpdateWithSideEffects({...state, repositories: Some(repos)}, (self) => getRepoIssues(repos, self))
      | GetRepoIssues (issues) => ReasonReact.Update({...state, issues: Some(issues)})
    },
    render: fun({state}) => {
      let repoItems = switch (state.repositories) {
        | Some(repos) => ReasonReact.arrayToElement (Array.map(
            (fun (repo: repository) => <ListItem key=repo.name name=repo.name url=repo.html_url issues=repo.open_issues_count />), repos)
          )
        | None => ReasonReact.stringToElement("Loading Repos...")
      };
      let issues  =switch(state.issues){
      | Some(issues) => ReasonReact.arrayToElement (Array.map((fun (issue: issue) => <ListItem key=string_of_int(issue.id) name=string_of_int(issue.id) url="" issues=0 />), issues))
      | None => ReasonReact.stringToElement("Loading Issues...")
      };
      <div className="App">
        <h2> (ReasonReact.stringToElement("Issues from Starred Repos")) </h2>
        <div>
          /* <LoginButton /> */
          <List>
            {repoItems}
            {issues}
          </List>
        </div>
      </div>
    }
  }
};
