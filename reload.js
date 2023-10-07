const fs = require("fs")
const { execFile, ChildProcess } = require("child_process")
const write = console.log
let process_handler
;(async () => {
	process_handler = await runProgram()
})()

let isProcessing = false
fs.watch("src", {}, async () => {
	if (isProcessing) return
	isProcessing = true
	if (process_handler) process_handler.kill("SIGINT")
	process_handler = await runProgram()
	isProcessing = false
})

async function runProgram() {
	let cmake = execFile("cmake", ["."])

	cmake.stdout.on("data", write)
	cmake.stderr.on("data", write)
	let exitCode = await awaitProcessExit(cmake)
	if (exitCode) return null

	let make = execFile("make")
	make.stdout.on("data", write)
	make.stderr.on("data", write)
	exitCode = await awaitProcessExit(make)
	if (exitCode) return null

	let project = execFile("./opengl_project")
	project.stdout.on("data", write)
	project.stderr.on("data", write)
	// await awaitProcessExit(project)

	return project
}

/**
 *
 * @param {ChildProcess} process
 * @returns
 */
function awaitProcessExit(process) {
	return new Promise((res) => {
		process.on("exit", (code) => {
			res(code)
		})
	})
}
