using dotenv.net;

namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        /// <summary>
        /// Creates or overwrites a ./.env file containing the placeholder token and notifies the user to add their token.
        /// </summary>
        /// <remarks>
        /// Writes the text "TOKEN=PASTE_YOUR_TOKEN" to ./.env and prints a message to the console prompting the user to add a real token.
        /// </remarks>
        private static void CreateEnvFile()
        {
            File.WriteAllText("./.env", "TOKEN=PASTE_YOUR_TOKEN");
            Console.WriteLine("Created env file. Please add your token in ./.env.");
        }
        /// <summary>
        /// Program entry point that ensures a valid TOKEN is available before proceeding.
        /// </summary>
        /// <remarks>
        /// If the ./.env file does not exist, a template .env file is created and the program exits.
        /// If the file exists, environment variables are loaded and the `TOKEN` value is checked;
        /// if `TOKEN` is missing, empty, or equals the placeholder `PASTE_YOUR_TOKEN`, a prompt is printed and the program exits.
        /// </remarks>
        private static void Main()
        {
            if (!File.Exists("./.env"))
            {
                CreateEnvFile();
                return;
            }
            
            DotEnv.Load();
            
            var token = Environment.GetEnvironmentVariable("TOKEN");
            
            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine("Please add your token in ./.env.");
                return;
            }
            
        }
    }
}