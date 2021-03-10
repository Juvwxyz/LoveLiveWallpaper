using System;

namespace LLWP
{
    class LLWP : LLEngine.Application
    {
        static void Main(string[] args)
        {
            LLWP app = new();
            app.Run();
        }

        protected override void CleanUp()
        {
        }

        protected override void StartUp()
        {
        }

        protected override void Update()
        {
        }
    }
}
