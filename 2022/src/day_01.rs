use anyhow::Result;
use itertools::Itertools;

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let result: i64 = lines
        .iter()
        .group_by(|line| line.is_empty())
        .into_iter()
        .filter(|(skip, _group)| !skip)
        .map(|(_, group)| {
            let x: i64 = group
                .into_iter()
                .map(|line| line.parse::<i64>().unwrap())
                .sum();
            x
        })
        .max()
        .unwrap_or_default();
    Ok(result)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let result: i64 = lines
        .iter()
        .group_by(|line| line.is_empty())
        .into_iter()
        .filter(|(skip, _group)| !skip)
        .map(|(_, group)| {
            let x: i64 = group
                .into_iter()
                .map(|line| line.parse::<i64>().unwrap())
                .sum();
            x
        })
        .sorted()
        .rev()
        .take(3)
        .sum();
    Ok(result)
}
