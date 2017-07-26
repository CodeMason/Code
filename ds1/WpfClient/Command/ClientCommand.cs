﻿//
// The MIT License(MIT)
//
// Copyright(c) 2014 Demonsaw LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

namespace DemonSaw.Command
{
	using DemonSaw.Component;
	using DemonSaw.Entity;
	using DemonSaw.Network;
	using System.Net.Sockets;

	public abstract class ClientCommand : DemonCommand
	{
		#region Properties		
		public ClientOptionsComponent Options { get { return Entity.Get<ClientOptionsComponent>(); } }
		
		public FolderListComponent FolderList { get { return Entity.Get<FolderListComponent>(); } }
		public SearchListComponent SearchList { get { return Entity.Get<SearchListComponent>(); } }

		public FolderMapComponent FolderMap { get { return Entity.Get<FolderMapComponent>(); } }
		public FileMapComponent FileMap { get { return Entity.Get<FileMapComponent>(); } }		
		#endregion

		#region Constructors
		public ClientCommand(Entity entity) : base(entity) { }
		public ClientCommand(Entity entity, Socket socket) : base(entity, socket) { }
		public ClientCommand(Entity entity, NetworkConnection connection) : base(entity, connection) { }
		#endregion
	}
}
