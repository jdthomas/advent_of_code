use anyhow::Result;
use itertools::Itertools;
use std::collections::HashSet;

fn score(c: &char) -> i64 {
    let i: i64 = *c as i64;
    match c {
        'a'..='z' => i - ('a' as i64) + 1,
        'A'..='Z' => i - ('A' as i64) + 27,
        _ => unimplemented!(),
    }
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    // println!("here: {:?}", lines);
    let result: i64 = lines
        .iter()
        .map(|line| {
            let (a, b) = line.split_at(line.len() / 2);
            let a: HashSet<char> = a.chars().collect();
            let b: HashSet<char> = b.chars().collect();
            let res = a.intersection(&b).map(score).sum::<i64>();
            // println!("a={:?} b={:?} score={}", a, b, res);
            res
        })
        .sum();
    Ok(result)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let res = lines
        .iter()
        .chunks(3)
        .into_iter()
        .map(|elf_group| {
            let mut elf_group = elf_group
                .into_iter()
                .map(|line| line.chars().collect::<HashSet<char>>());
            let first = elf_group.next().unwrap();
            let badge = elf_group.fold(first, |acc, hs| acc.intersection(&hs).cloned().collect());
            // println!("badge={:?} ", badge);
            assert!(badge.len() == 1);
            badge.iter().map(score).sum::<i64>()
        })
        .sum();
    Ok(res)
}
